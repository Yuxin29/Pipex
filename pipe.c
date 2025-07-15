/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuwu <yuwu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 19:47:21 by yuwu              #+#    #+#             */
/*   Updated: 2025/06/28 12:01:39 by yuwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* 
-------------------new functions and knowledges here----------------------------
fd_tables:
0  → terminal input		(stdin)
1  → terminal outpu		(stdout)
2  → terminal error		(stderr)

dup2()
it creates a copy of the file descriptor oldfd, 
using the file descriptor number specified in newfd.  
* If oldfd is not valid, then the call fails, and newfd is not closed.
* If oldfd is valid, and newfd has the same value as oldfd,
  then dup2() does nothing, and returns newfd.
PRPTOTYPE			int dup2(int oldfd, int newfd);

--------------------------------------------------------------------------------
					a tree structure of how the main.c work
--------------------------------------------------------------------------------
|						    Parent Process 									   |
|						   	   {main}										   |
|                   		(creates pipe)									   |
|                        		|											   |
|	         		-----------------------------------						   |
|   	    		|                        	       |					   |
|			{first_fork} 							{second_fork} 			   |
|			[child1_pid]   	        				[child2_pid]			   |
|    	    		|                   	           |					   |
|	  ------------------------	     			---------------------		   |
|	  |          	         |	         		|                   |		   |
|	Child 1           		Parent	     	Child 2            Parent		   |
| (exec cmd1)     		 after fork1  	   (exec cmd2)       after fork2	   |
| 	  |                  	 |     		    	|                    |		   |
|	  |                   	 |         			|                    |		   |
|	┌--------------- --─┐    |   	  	┌----------------────┐   	 |		   |
|	│ close(pipe_fd[0]) │  	 |     		│ close(pipe_fd[1])  │	     |		   |
|	│ dup2(infile, 0)   │  	 |	     	│ dup2(pipe_fd[0], 0)│   	 |		   |
|	│ dup2(pipe_fd[1],1)│ 	 | 	    	│ dup2(outfile, 1)   │   	 |	       |
|	│   {exe_cmd1}      |    |          |   {exe_cmd2}       |       |		   |
|	|execve(grep hello) │  	 |     		│ execve(wc -l)      │   	 |		   |
|	└-----------------──┘  	 |    		└----------------────┘   	 |		   |
|    	                  	 |                              		 |		   |
|							{main}									 |	       |
|					 Parent closes both ends of pipe                 |		   |
|            	     waitpid(child1_pid)                             |		   |
|                	 waitpid(child2_pid)							 |		   |
-------------------------------------------------------------------------------|
					a liner structure of how the pipe work
--------------------------------------------------------------------------------
|			fd0			 fd1     /-------/\		fd0			  fd1			   |
| infile  ----->  cmd1  ----->  |  pipe |  |  ----->   cmd2  ------>   outfile |
|       (infile_fd)   pipefd[1]  \ _____ \/ (pipefd[0])  (outfile_fd)   	   |
|                     write_side		      read_side                        |
|                             \             / 							   	   |
|                              \ pipefd[2] /							       |
|                               /           \		   					  	   |
|                         first_fork     	second_fork					       |
|                           (cmd1)           (cmd2)				   		       |
|                         (child 1)        (child 2) 					       |
|						  /    |			|		\						   |
|						 / 	    \		   /		 \						   |
|						/	     |		  |			  \ 					   |
|					   /         |        |            |                       |
|			dup2(infile_fd, 0) dup2(pipefd[1], 1)      |
|(Redirect stdin to infile_fd) (Redirect stdout to pipe write)    			   |
|							              |            |					   |
|										 /             |					   |
|										/			   |					   |
|               	    dup2(pipefd[0], 0)  	dup2(outfile_fd, 1)			   |
|            (Redirect stdin to pipe read)  	(Redirect stdout to outfile)   |
--------------------------------------------------------------------------------

execve(cmd_line[0], cmd_line, envp); 
If execve returns, it failed
| Exit Code | Meaning                 |
| --------- | ----------------------- |
| `127`     | Command not found       |
| `126`     | Permission denied       |
| `1`       | General execution error |
| `0`       | Successful Execution    |
| `0`       | Second Command Succeeds |
| `0`       | SFile Operations Succeed|

-------------------------------------------------------------------------------
defensive closing all fds from 3 to 1023 to avoid Accidental FD Inheritance
close_fds_from(3) goes through and closes everything from fd 3 to 1023
-------------------------------------------------------------------------
yes | grep something: yes writes to its stdout forever.
close_fds_from(3) ensures each process only holds what it needs 
— this allows pipes to close properly and writers to die on SIGPIPE.
---------------------------------------------------------------------------

defensive reasoning for early exit on open null fd failure
- the Failure here indicates a severe system-level error
- an immediate early exit is justified. 
- This ensures no undefined behavior or orphaned child processes occur. 
- The timing impact is negligible due to the rarity of this failure.
- The correctness, safety, and reliability of a program 
- OUTWEIGH the minor timing difference caused by early exit here.
-----------------------------------------------------------------------------
defensive fd check:
env -i PATH=/bin:/usr/bin ./pipex Makefile cat cat outf

static void	safe_close(int fd)
{
	if (fd >= 0)
		close(fd);
	fd = -1;

*/

#include "pipex.h"

//defensicve closing
static void	close_fds_from(int start)
{
	int	i;

	i = start;
	while (i < 1024)
	{
		close(i);
		i++;
	}
}

//fds given as 2 int array
//change the fds and return an int signal: 1 as failures and 0 as success
//out_no_write: outfile exists but not writable
static int	init_fds(int *fds, char **av)
{
	int	out_no_write;

	out_no_write = (access(av[4], F_OK) == 0 && access(av[4], W_OK) == -1);
	fds[0] = open(av[1], O_RDONLY);
	if (fds[0] < 0)
	{
		error_msg("pipex: ", av[1], ": Permission denied\n");
		fds[0] = open("/dev/null", O_RDONLY);
	}
	if (!out_no_write)
		fds[1] = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (out_no_write)
	{
		error_msg("pipex: ", av[4], ": Permission denied\n");
		if (fds[1] < 0)
			fds[1] = open("/dev/null", O_WRONLY);
	}
	if (fds[0] < 0 || fds[1] < 0)
		ft_putstr_fd("pipex: open /dev/null failed\n", 2);
	if (fds[0] < 0 || fds[1] < 0 || out_no_write)
		return (1);
	return (0);
}

//one fork for 2 process, adust the av and fds when called 
//On  success, 0 is returned in child, child process is returned in the parent;
//On failure, -1 is returned, no child process is created
//error msgs here: fork and dup2 failure, not cmd or file err msgs here
static pid_t	ft_fork(int input_fd, int output_fd, char *cmd, char **envp)
{
	pid_t	pid;
	int		result;

	pid = fork();
	if (pid == -1)
		ft_putstr_fd("pipex: fork failed\n", 2);
	if (pid == 0)
	{
		if (dup2(input_fd, 0) == -1 || dup2(output_fd, 1) == -1)
			ft_putstr_fd("pipex: dup2 failed\n", 2);
		close_fds_from(3);
		result = exe_cmd(cmd, envp);
		exit(result);
	}
	close_pair(input_fd, output_fd);
	return (pid);
}

//here is the pipe process and child2
// Here I first check initd_fds erros and save it as wait_status[0]
// and then check 2nd cmd existence,
// Otherwise keep the status from the last command
void	execute_pipeline(char **av, char **envp, int *wait_status, int *fds)
{
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;

	if (pipe(pipefd) == -1)
		close_and_error(fds, 0, "pipex: pipe failed\n", 127);
	wait_status[1] = check_command_existence(av[2], envp);
	if (wait_status[1] == 1)
		pid1 = ft_fork(fds[0], pipefd[1], av[2], envp);
	else
	{
		error_126(wait_status[1], av[2]);
		pid1 = ft_fork(fds[0], pipefd[1], "true", envp);
	}
	wait_status[1] = check_command_existence(av[3], envp);
	if (wait_status[1] == 1)
		pid2 = ft_fork(pipefd[0], fds[1], av[3], envp);
	else
	{
		error_126(wait_status[1], av[3]);
		pid2 = ft_fork(pipefd[0], fds[1], "true", envp);
	}
	close_pair(pipefd[0], pipefd[1]);
	waitpid(pid1, &wait_status[2], 0);
	waitpid(pid2, &wait_status[3], 0);
}

//main
//fds[2] as stack mem and initiate here.
// wait_status is an int array of storing status codes used for final exit.
// wait_status[0] : Result of init_fds() — initialization of input/output files.
// wait_status[1] : Status of the cmd1 existence check. reused for cmd2 too
// wait_status[2] : Raw exit status of the child1 process (cmd1). unchecked here
// wait_status[3] : Raw exit status of the child2 process (cmd2).
int	main(int ac, char **av, char **envp)
{
	int	fds[2];
	int	wait_status[4];

	if (ac != 5)
		close_and_error(0, 0, "Usage: ./pipex infile cmd1 cmd2 outfile\n", 1);
	fds[0] = -1;
	fds[1] = -1;
	wait_status[0] = init_fds(fds, av);
	execute_pipeline(av, envp, wait_status, fds);
	close_pair(fds[0], fds[1]);
	if (wait_status[0])
		exit(1);
	if (wait_status[1] == 126 || wait_status[3] == 126)
		exit(126);
	else if (wait_status[1] == 127 || wait_status[3] == 127)
		exit(127);
	exit(WEXITSTATUS(wait_status[3]));
}
