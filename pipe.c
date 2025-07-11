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
| `0`       | Second Command Succeeds:   |
| `0`       | SFile Operations Succeed:   |

*/

#include "pipex.h"

//initiate file descripter.
//fds given as 2 int array, malloced in main, so no malloc failuere here
//return an in as error signal
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
	{
		error_msg("pipex: ", av[4], ": Permission denied\n");
		fds[1] = open("/dev/null", O_WRONLY);
	}
	else
	{
		fds[1] = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fds[1] < 0)
		{
			error_msg("pipex: ", av[4], ": Permission denied\n");
			fds[1] = open("/dev/null", O_WRONLY);
		}
	}
	if (fds[0] < 0 || fds[1] < 0 || out_no_write)
		return (1);
	return (0);
}

//I have only one fork for 2 process, just adust the av and fd when called 
//On  success, 0 is returned in child, child process is returned in the parent;
//On failure, -1 is returned, no child process is created
//exit status 1 possible because fork might fail, be no cmd checking here.
//but here it exits only child process
pid_t	ft_fork(int input_fd, int output_fd, char *cmd, char **envp)
{
	pid_t	pid;
	int		result;

	pid = fork();
	if (pid == -1)
	{
		close(input_fd);
		close(output_fd);
		close_and_error(0, 0, "pipex: fork failed\n", 1);
	}
	if (pid == 0)
	{
		if (dup2(input_fd, 0) == -1 || dup2(output_fd, 1) == -1)
		{
            close(input_fd);
            close(output_fd);
            close_and_error(0, 0, "pipex: dup2 failed\n", 1);
        }
		close(input_fd);
		close(output_fd);
		result = exe_cmd(cmd, envp);
		exit(result);
	}
	close(input_fd);
	close(output_fd);
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
	int		status1;
	int		status2;
	int		file_error;
	int		cmd1_status;
	int		cmd2_status;

	file_error = init_fds(fds, av);
	if (pipe(pipefd) == -1)
		close_and_error(fds, 0, "pipex: pipe failed\n", 127);
	cmd1_status = check_command_existence(av[2], envp);
	if (cmd1_status == 1)
		pid1 = ft_fork(fds[0], pipefd[1], av[2], envp);
	else
	{
		if (cmd1_status == 126)
			error_msg("pipex: ", av[2], ": Permission denied\n");
		else
			error_msg("pipex: ", av[2], ": command not found\n");
		int null_fd = open("/dev/null", O_RDONLY);
		if (null_fd < 0)
			close_and_error(fds, 0, "pipex: open /dev/null failed\n", 1);
		pid1 = ft_fork(null_fd, pipefd[1], "true", envp);
	}
	close(pipefd[1]);
	cmd2_status = check_command_existence(av[3], envp);
	if (cmd2_status == 1)
		pid2 = ft_fork(pipefd[0], fds[1], av[3], envp);
	else
	{
		if (cmd2_status == 126)
			error_msg("pipex: ", av[3], ": Permission denied\n");
		else
			error_msg("pipex: ", av[3], ": command not found\n");
		int null_fd = open("/dev/null", O_RDONLY);
		if (null_fd < 0)
			close_and_error(fds, 0, "pipex: open /dev/null failed\n", 1);
		pid2 = ft_fork(pipefd[0], null_fd, "true", envp);
	}
	close(pipefd[0]);
	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	if (file_error)
		*wait_status = 1;
	else if (cmd2_status == 126)
		*wait_status = 126;
	else if (cmd2_status == 127)
		*wait_status = 127;
	else if (WIFEXITED(status2))
		*wait_status = WEXITSTATUS(status2);
	//else if (cmd2_status == 126 || (WIFEXITED(status2) && WEXITSTATUS(status2) == 126))
	//	*wait_status = 126;
	else
		*wait_status = 1;
	close(fds[0]);
	close(fds[1]);
}

//fds[2] malloced and preset here at beginning and exit ealy here if fails
//at the end:
//WIFEXITED		Normal exit			Child's exit() code		Common (0 or 127)
//WIFSIGNALED	Killed by signal	128 + signal number		Crashes
//otherwise		raw status
int	main(int ac, char **av, char **envp)
{
	int	*fds;
	int	wait_status;

	if (ac != 5)
		close_and_error(0, 0, "Usage: ./pipex infile cmd1 cmd2 outfile\n", 1);
	fds = malloc(sizeof(int) * 2);
	if (!fds)
		close_and_error(fds, 0, "malloc error\n", 1);
	fds[0] = -1;
	fds[1] = -1;
	execute_pipeline(av, envp, &wait_status, fds);
	free(fds);
	exit(wait_status);
}
