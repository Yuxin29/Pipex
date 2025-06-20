/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuwu <yuwu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 19:47:21 by yuwu              #+#    #+#             */
/*   Updated: 2025/06/19 19:48:20 by yuwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ----------------examples------------------------
./pipex infile "grep hello" "wc -l" outfile
------------------------------------------------------------
                 Parent Process (main)
                    (creates pipe)
                        |
          ---------------------------------
          |                               |
       fork() [child1_pid]            fork() [child2_pid]
          |                               |
  ---------------------         ---------------------
  |                   |         |                   |
Child 1           Parent      Child 2            Parent
(exec cmd1)      after fork1  (exec cmd2)       after fork2
  |                   |         |                    |
  |                   |         |                    |
┌----------------─┐   |     ┌----------------────┐   |
│ close(pipe_fd[0])│  |     │ close(pipe_fd[1])  │   |
│ dup2(infile, 0)  │  |     │ dup2(pipe_fd[0], 0)│   |
│ dup2(pipe_fd[1],1)│ |     │ dup2(outfile, 1)   │   |
│ execve(cmd1)     │  |     │ execve(cmd2)       │   |
└----------------──┘  |     └----------------────┘   |
                      |                              |
                 Parent closes both ends of pipe     |
                 waitpid(child1_pid)                 |
                 waitpid(child2_pid)
                 wifexited() //macros                |
---------------------------------------------------
*/

#include "pipex.h"

static void first_fork(int infile_fd, int pipefd[2], char *cmd1, char **envp)
{
    pid_t fk1;

    fk1 = fork();
    if (fk1 == -1) ///failure
    {
        //"fork error";
        return ;
    }
    else
    {
        // In child1: grep hello
        // stdin ← infile
        // stdout → pipe write
        // Close unused read end
        // av[2] = "grep hello"  
        return ; 
    }
}

static void second_fork(int outfile_fd, int pipefd[2], char *cmd2, char **envp)
{
    // --- Second fork: cmd2 ---
        // In child2: wc -l
        // stdin ← pipe read
        // stdout → outfile
        // Close unused write end
        // av[3] = "wc -l"
}

// Find PATH=... from envp
// this is not a static, it is going to be called in every cmd/
char *find_path_in_envp(char **envp)
{
    // example: envp = {"SHELL=/bin/bash", "PATH=/usr/local/bin:/usr/bin:/bin", NULL}
    //output : "/usr/local/bin:/usr/bin:/bin"
    //grep could be: 
    - /usr/local/bin/grep
    - /usr/bin/grep
    - /bin/grep
    //just grabbing the raw string that tells you where the system will search for commands.
}

//main process, excution function
int main(int ac, char **av, char **envp)
{
    // Check argument count
    if (ac != 5 || strcmp((const char)"pipex", (const char)av[0]) == 1)
        return (0);

    // Open files
    if (open_infile((const char)av[1]) != 0)
        return (1); //this is my error massage, can it be???
    if (open_outfile((const char)av[4]) != 1)
        return (1);
    
    // pipe: create pipe
    int check_pipe;
    int pipefd[2]

    check_pipe = pipe(pipefd);
    if (check_pipe = -1)
    {
        perror("pipe failed");
        return (1);
    }

    // --- First fork: cmd1 ---
    first_fork(int infile_fd, int pipefd[2], char *cmd1, char **envp);

    // --- Second fork: cmd2 ---
    second_fork(int outfile_fd, int pipefd[2], char *cmd2, char **envp);

    // Parent process, cloese fds, wait chilren, clean up and exit
    xxx;

    return (0);
}

