/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuwu <yuwu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 19:47:21 by yuwu              #+#    #+#             */
/*   Updated: 2025/06/22 17:25:42 by yuwu             ###   ########.fr       */
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
|                               /      |     \		   					  	   |
|                         first_fork   |  	second_fork					       |
|                           (cmd1)     |      (cmd2)				           |
|                         (child 1)    |    (child 2) 					       |
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

*/

#include "pipex.h"

// In child1: grep hello
// infile → stdin
// pipe write → stdout
// close read end of the pipe (not needed in cmd1)
// close write end of the pipe (handled by dup2)
static pid_t	fork_1(int infile_fd, int pipefd[2], char *av2, char **envp)
{
	pid_t	fk1;

	fk1 = fork();
	if (fk1 == -1)
		ft_error("Error fork");
	if (fk1 == 0)
	{
		if (infile_fd >= 0)
		{
			dup2(infile_fd, 0);
			close(infile_fd);
		}
		dup2(pipefd[1], 1);
		close(pipefd[0]);
		close(pipefd[1]);
		exe_cmd(av2, envp);
	}
	return (fk1);
}

// In child2: wc -l
// read end → stdin
// outfile → stdout
// close write end of the pipe (not needed in cmd2)
// close read end of the pipe (handled by dup2)
static pid_t	fork_2(int outfile_fd, int pipefd[2], char *av3, char **envp)
{
	pid_t	fk2;

	fk2 = fork();
	if (fk2 == -1)
		ft_error("Error fork");
	if (fk2 == 0)
	{
		dup2(pipefd[0], 0);
		dup2(outfile_fd, 1);
		close(pipefd[1]);
		close(pipefd[0]);
		exe_cmd(av3, envp);
	}
	return (fk2);
}

static void	init_fds(char **av, int *infile_fd, int *outfile_fd)
{
	*outfile_fd = open_file(av[4], 1);
	if (*outfile_fd < 0)
	{
		perror("pipex: output file error");
		exit(1);
	}
	*infile_fd = open_file(av[1], 0);
	if (*infile_fd < 0)
	{
		perror("pipex: input file error");
		close(*outfile_fd);
		exit(1);
	}
}

static void	execute_pipeline(char **av, char **envp, int *status1, int *status2)
{
	int		infile_fd;
	int		outfile_fd;
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;

	init_fds(av, &infile_fd, &outfile_fd);
	if (pipe(pipefd) == -1)
	{
		close(infile_fd);
		close(outfile_fd);
		ft_error("pipe failed");
	}
	pid1 = fork_1(infile_fd, pipefd, av[2], envp);
	pid2 = fork_2(outfile_fd, pipefd, av[3], envp);
	close_all_four(infile_fd, outfile_fd, pipefd);
	waitpid(pid1, status1, 0);
	waitpid(pid2, status2, 0);
}

int	main(int ac, char **av, char **envp)
{
	int	status1;
	int	status2;

	if (ac != 5)
		return (write(2, "Usage: ./pipex infile cmd1 cmd2 outfile\n", 39), 1);
	execute_pipeline(av, envp, &status1, &status2);
	return (handle_exit_status(status1, status2));
}
