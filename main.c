/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuwu <yuwu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 19:47:21 by yuwu              #+#    #+#             */
/*   Updated: 2025/06/26 13:54:48 by yuwu             ###   ########.fr       */
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

exit(127); // Command not found
exit(126); // Permission denied
exit(1);		//execution error

*/

#include "pipex.h"

//initiate file descripter.
//fds given as 2 int array, asigned in the function
//return an in as errir cide
int	init_fds(int *fds, char **av)
{
	int	error;

	error = 0;
	fds[0] = open(av[1], O_RDONLY);
	if (fds[0] < 0)
	{
		perror("pipex: input file");
		fds[0] = open("/dev/null", O_RDONLY);
		if (fds[0] < 0)
			error = 1;
	}
	if (access(av[4], F_OK) == 0 && access(av[4], W_OK) == -1)
	{
		perror("pipex: output file (no permission)");
		error = 1;
		fds[1] = open("/dev/null", O_WRONLY);
	}
	else
	{
		fds[1] = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fds[1] < 0)
		{
			perror("pipex: output file open failed");
			error = 1;
			fds[1] = open("/dev/null", O_WRONLY);
		}
	}
	if (fds[1] < 0)
		error = 1;
	return (error);
}

//I have one fork only for 2 childen, just adust the av when called 
//On  success, 0 is returned in child, child process is returned in the parent;
//On failure, -1 is returned, no child process is created
//exit status 1 possible because fork might fail, be no cmd checking here.
pid_t	ft_fork(int input_fd, int output_fd, char *cmd, char **envp)
{
	pid_t	pid;
	int		result;

	pid = fork();
	if (pid == -1)
		close_and_error(0, 0, "fork", 1);
	if (pid == 0)
	{
		if (dup2(input_fd, 0) == -1 || dup2(output_fd, 1) == -1)
			close_and_error(0, 0, "pipex: dup2 failed", 1);
		close(input_fd);
		close(output_fd);
		result = exe_cmd(cmd, envp);
		exit(result);
	}
	close(input_fd);
	close(output_fd);
	return (pid);
}

// Otherwise keep the status from the last command
void execute_pipeline(char **av, char **envp, int *wait_status, int *fds)
{
	int     pipefd[2];
	pid_t   pid1;
	pid_t   pid2;
	int     status1;
	int     file_error;

	file_error = 0;
	if (init_fds(fds, av))
		file_error = 1;
	if (pipe(pipefd) == -1)
		close_and_error(fds, pipefd, "pipe failed", 127);
	if (file_error) 
	{
		int input_fd = open("/dev/null", O_RDONLY);
		if (check_command_existence(av[2], envp) == 1)
			pid1 = ft_fork(input_fd, pipefd[1], av[2], envp);
		else
			pid1 = ft_fork(open("/dev/null", O_RDONLY), pipefd[1], "true", envp);
	} 
	else 
	{
		if (check_command_existence(av[2], envp) == 1)
			pid1 = ft_fork(fds[0], pipefd[1], av[2], envp);
		else
			pid1 = ft_fork(open("/dev/null", O_RDONLY), pipefd[1], "true", envp);
	}
	close(pipefd[1]);
	if (file_error) 
	{
		int output_fd = open("/dev/null", O_WRONLY);
		if (check_command_existence(av[3], envp) == 1)
			pid2 = ft_fork(pipefd[0], output_fd, av[3], envp);
		else
			pid2 = ft_fork(pipefd[0], open("/dev/null", O_WRONLY), "true", envp);
	} 
	else
	{
		if (check_command_existence(av[3], envp) == 1)
			pid2 = ft_fork(pipefd[0], fds[1], av[3], envp);
		else
			pid2 = ft_fork(pipefd[0], open("/dev/null", O_WRONLY), "true", envp);
	}
	close(pipefd[0]);
	waitpid(pid1, &status1, 0);
	waitpid(pid2, wait_status, 0);
	close(fds[0]);
	close(fds[1]);
	if (file_error)
		*wait_status = W_EXITCODE(1, 0);
	else if (!check_command_existence(av[3], envp))
		*wait_status = W_EXITCODE(127, 0);
}

//fds[2] malloced and preset here at beginning and exit ealy here if fails
//at the end:
//WIFEXITED		Normal exit			Child's exit() code		Common (0 or 127)
//WIFSIGNALED	Killed by signal	128 + signal number		Crashes
//otherwise		raw status
int	main(int ac, char **av, char **envp)
{
	int	*fds;
	int	final_exit_code = 1; // what we return from main()
	int	wait_status;

	fds = malloc(sizeof(int) * 2);
	if (!fds)
		close_and_error(fds, 0, "malloc error", 1);
	fds[0] = -1;
	fds[1] = -1;
	if (ac != 5)
		close_and_error(fds, 0, "Usage: ./pipex infile cmd1 cmd2 outfile", 127);
	if (!*av[1] || !*av[2] || !*av[3] || !*av[4])
		close_and_error(fds, 0, "pipex: missing or empty command", 127);
	execute_pipeline(av, envp, &wait_status, fds);
	free(fds);
	if (WIFEXITED(wait_status))
		final_exit_code = WEXITSTATUS(wait_status);
	else if (WIFSIGNALED(wait_status))
		final_exit_code = 128 + WTERMSIG(wait_status);
	else
		final_exit_code = wait_status;
	return (final_exit_code);
}
