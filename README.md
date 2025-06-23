# Pipex

prelearning for the later project minishell


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
|			fd0			fd1	     /-------/\		fd0			  fd1			   |
| infile  ----->  cmd1  ----->  |  pipe |  |  ----->   cmd2  ------>   outfile |
|       (infile_fd)   pipefd[1]  \ _____ \ / (pipefd[0])  (outfile_fd)   	   |
|                     write_side		      read_side                        |
|                             \             / 							   	   |
|                              \ pipefd[2] /							       |
|                               /      |     \		   					  	   |
|                         first_fork   |  	second_fork					       |
|                           (cmd1)     |      (cmd2)				           |
|                         (child 1)    |    (child 2) 					       |
--------------------------------------------------------------------------------

---------------------new variable types used here--------------------------------
- pid_t is a data type used in POSIX systems (like Linux and macOS) to represent process IDs. 
    - pid_t is a typedef (an alias) for some integer type, usually a signed integer like int.
    - It is the type returned by process-related system calls such as fork(), getpid(), and waitpid().

---------------------allower external functions-----------------------
open, close, read, write, malloc, free, perror, strerror, access, dup, dup2,
execve, exit, fork, pipe, unlink, wait, waitpid

--------------below are what are new to me---------------
- xdg-open: opens a file or URL in the user's preferred application
    xdg-open {file | URL}
- close: close a file descriptor
    int close(int fd);
- read: read — read from standard input into shell variables
- perror - print a system error message
   void perror(const char *s);
- strerror: return string describing error number
    char *strerror(int errnum);
- access: check user's permissions for a file
    int access(const char *pathname, int mode);
- dup, dup2: duplicate a file descriptor
    int dup(int oldfd);
    int dup2(int oldfd, int newfd);
- execve - execute program
    int execve(const char *pathname, char *const argv[], char *const envp[]);
- exit: cause the shell to exit
- fork
    pid_t fork(void);
    On success, the PID of the child process is returned in the parent, and 0 is returned  in  the  child.
       On failure, -1 is returned in the parent, no child process is created, and errno is set appropriately.
- pipe: Creates a unidirectional communication channel (a pipe) between processes
    usually between a parent and child after a fork().
    - pipefd[0]: the read end of the pipe
    - pipefd[1]: the write end of the pipe
    sysnoiphasise: int  pipe(int pipefd[2]);  
                   fd[2]: an array of 2 ints
                   -return values: 0 on success and -1 on fai
- unlink: call  the  unlink function to remove the specified file
- wait, waitpid, waitid - wait for process to change state
    #include <sys/types.h>
    #include <sys/wait.h>
    pid_t wait(int *wstatus);
    pid_t waitpid(pid_t pid, int *wstatus, int options);
