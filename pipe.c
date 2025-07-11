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
it creates a copy of the file descriptor 
oldfd, using the file descriptor number specified in newfd.  
If the file descriptor newfd was previously open, 
it is silently closed before being reused.
* If oldfd is not valid, then the call fails, and newfd is not closed.
* If oldfd is valid, and newfd has the same value as oldfd,
  then dup2() does nothing, and returns newfd.
PRPTOTYPE
int dup(int oldfd);
int dup2(int oldfd, int newfd);
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
|                	 wifexited() //macros                            |		   |
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

execve(cmd_line[0], cmd_line, envp); // If execve returns, it failed

| Exit Code | Meaning                 |
| --------- | ----------------------- |
| `127`     | Command not found       |
| `126`     | Permission denied       |
| `1`       | General execution error |
| `0`       | Successful Execution    |
| `0`       | Second Command Succeeds |
| `0`       | SFile Operations Succeed|

*/

#include "pipex.h"

//initiate file descripter.
//fds given as 2 int array, malloced in main, so no malloc failuere here
//return an int as error signal: 1 as failures and 0 as success
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
	if (out_no_write)
		error_msg("pipex: ", av[4], ": Permission denied\n");
	if (!out_no_write)
	{
		fds[1] = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fds[1] < 0)
			fds[1] = open("/dev/null", O_WRONLY);
	}
	if (fds[0] < 0 || fds[1] < 0)
		return (error_msg("pipex: ", NULL, ": open null failed\n"), 1);
	if (out_no_write)
		return (1);
	return (0);
}

//I have one fork for 2 process, adust the av and fds when called 
//On  success, 0 is returned in child, child process is returned in the parent;
//On failure, -1 is returned, no child process is created
//error msgs here: fork and dup2 failure
static pid_t	ft_fork(int input_fd, int output_fd, char *cmd, char **envp)
{
	pid_t	pid;
	int		result;

	pid = fork();
	if (pid == -1)
	{
		close_pair(input_fd, output_fd);
		ft_putstr_fd("pipex: fork failed\n", 2);
	}
	if (pid == 0)
	{
		if (dup2(input_fd, 0) == -1 || dup2(output_fd, 1) == -1)
		{
			close_pair(input_fd, output_fd);
			ft_putstr_fd("pipex: dup2 failed\n", 2);
		}
		close_pair(input_fd, output_fd);
		result = exe_cmd(cmd, envp);
		exit(result);
	}
	close_pair(input_fd, output_fd);
	return (pid);
}

//first child
static pid_t	child1(char **av, char **envp, int input_fd, int pipe_write)
{
	pid_t	pid;
	int		cmd_status;
	int		null_fd;

	cmd_status = check_command_existence(av[2], envp);
	if (cmd_status == 1)
		return (ft_fork(input_fd, pipe_write, av[2], envp));
	error_126(cmd_status, av[2]);
	null_fd = open("/dev/null", O_RDONLY);
	if (null_fd < 0)
		close_and_error(0, 0, "pipex: open /dev/null failed\n", 1);
	pid = ft_fork(null_fd, pipe_write, "true", envp);
	close(null_fd);
	return (pid);
}

// Here I first check initd_fds erros and save it as file error
// here I first precheck cmd existence
// if yes I fork it.
// if not I still go fork a true cmd
// at the end I firt check init errors
// and then check 2nd cmd existence,
// Otherwise keep the status from the last command
void execute_pipeline(char **av, char **envp, int *wait_status, int *fds)
{
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;

	wait_status[0] = init_fds(fds, av);
	if (pipe(pipefd) == -1)
		close_and_error(fds, 0, "pipex: pipe failed\n", 127);
	pid1 = child1(av, envp, fds[0], pipefd[1]);
	close(pipefd[1]);
	wait_status[1] = check_command_existence(av[3], envp);
	if (wait_status[1] == 1)
		pid2 = ft_fork(pipefd[0], fds[1], av[3], envp);
	else
	{
		error_126(wait_status[1], av[3]);
		pid2 = ft_fork(pipefd[0], fds[1], "true", envp);
	}
	close(pipefd[0]);
	waitpid(pid1, &wait_status[2], 0);
	waitpid(pid2, &wait_status[3], 0);
	close_pair(fds[0], fds[1]);
}


//fds[2] malloced and preset here at beginning and exit ealy here if fails
//at the end:
//WIFEXITED		Normal exit			Child's exit() code		Common (0 or 127)
//WIFSIGNALED	Killed by signal	128 + signal number		Crashes
//otherwise		raw status
int	main(int ac, char **av, char **envp)
{
	int	*fds;
	int	wait_status[4];

	if (ac != 5)
		close_and_error(0, 0, "Usage: ./pipex infile cmd1 cmd2 outfile\n", 1);
	fds = malloc(sizeof(int) * 2);
	if (!fds)
		close_and_error(fds, 0, "malloc error\n", 1);
	fds[0] = -1;
	fds[1] = -1;
	execute_pipeline(av, envp, wait_status, fds);
	free(fds);
	if (wait_status[0])
		exit(1);
	else if (wait_status[1] == 126 || wait_status[3] == 126)
		exit(126);
	else if (wait_status[1] == 127 || wait_status[3] == 127)
		exit(127);
	if (WIFEXITED(wait_status[3]))
		exit(WEXITSTATUS(wait_status[3]));
	else if (WIFSIGNALED(wait_status[3]))
		exit(128 + WTERMSIG(wait_status[3]));
	exit(EXIT_FAILURE);
}