/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuwu <yuwu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 19:47:21 by yuwu              #+#    #+#             */
/*   Updated: 2025/06/21 15:25:53 by yuwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* 
-------------------new functions and knowledges here----------------------------
fd_tables:
0  → terminal input		(stdin)
1  → terminal outpu		(stdout)
2  → terminal error		(stderr)

dup2()
it creates a copy of the file descriptor oldfd, using the file descriptor number specified in newfd.  
If the file descriptor newfd was previously open, it is silently closed before being reused.
The  steps  of  closing  and reusing the file descriptor newfd are performed atomically.  
    * If oldfd is not a valid file descriptor, then the call fails, and newfd is not closed.
    * If oldfd is a valid file descriptor, and newfd has the same value as oldfd, then dup2() does nothing, and returns newfd.

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
|			fd0			 fd1    /-------/\		fd0			  fd1			   |
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
|dup2(infile_fd, STDIN_FILENO) dup2(pipefd[1], STDOUT_FILENO)                  |
|(Redirect stdin to infile_fd) (Redirect stdout to pipe write)    			   |
|							              |            |					   |
|										 /             |					   |
|										/			   |					   |
|                dup2(pipefd[0], STDIN_FILENO)  dup2(outfile_fd, STDOUT_FILENO)|
|                (Redirect stdin to pipe read)  (Redirect stdout to outfile)   |
--------------------------------------------------------------------------------

*/

#include "pipex.h"

// In child1: grep hello
// infile → stdin
// pipe write → stdout
// close read end of the pipe (not needed in cmd1)
// close write end of the pipe (handled by dup2)
static void	first_fork(int infile_fd, int pipefd[2], char *av2, char **envp)
{
	pid_t	fk1;

	fk1 = fork();
	if (fk1 == -1)
		ft_error("Error fork");
	if (fk1 == 0)
	{
		dup2(infile_fd, STDIN_FILENO);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
		exe_cmd(infile_fd, av2, pipefd[1], envp);
	}
}

// In child2: wc -l
// read end → stdin
// outfile → stdout
// close write end of the pipe (not needed in cmd2)
// close read end of the pipe (handled by dup2)
static void	second_fork(int outfile_fd, int pipefd[2], char *av3, char **envp)
{
	pid_t	fk2;

	fk2 = fork();
	if (fk2 == -1)
		ft_error("Error fork");
	if (fk2 == 0)
	{
		dup2(pipefd[0], STDIN_FILENO);
		dup2(outfile_fd, STDOUT_FILENO);
		close(pipefd[1]);
		close(pipefd[0]);
		exe_cmd(pipefd[0], av3, outfile_fd, envp);
	}
}

//main process, excution function
//esim      ./pipex infile "grep hello" "wc -l" outfile
// - Check argument count;
// - Open files nadh check
// - create pipe and check			// 0-OK; -1-KO;
// - fork: generates 2 fk process  
// - cloese fds, wait chilren, clean up and exit
int	main(int ac, char **av, char **envp)
{
	int		infile_fd;
	int		outfile_fd;
	int		pipefd[2];

	if (ac != 5)
		return (write(2, "Usage: ./pipex infile cmd1 cmd2 outfile\n", 40), 1);
	infile_fd = open_infile((const char *)av[1]);
	if (infile_fd < 0)
		return (1);
	outfile_fd = open_outfile((const char *)av[4]);
	if (outfile_fd < 0)
		return (close(infile_fd), 1);
	if (pipe(pipefd) == -1)
	{
		close(infile_fd);
		close(outfile_fd);
		ft_error("pipe failed");
	}
	first_fork(infile_fd, pipefd, av[2], envp);
	second_fork(outfile_fd, pipefd, av[3], envp);
	close_all_four(infile_fd, outfile_fd, pipefd);
	wait(NULL);
	wait(NULL);
	return (0);
}

//send error msg and exit
void	ft_error(char *error_msg)
{
	perror(error_msg);
	exit(EXIT_FAILURE);
}

//close fds and pipefd
void	close_all_four(int fd1, int fd2, int ppfd[2])
{
	close(fd1);
	close(fd2);
	close(ppfd[0]);
	close(ppfd[1]);
}
