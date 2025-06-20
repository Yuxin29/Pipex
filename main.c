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
  |                   |         |                   |
  |                   |         |                   |
┌----------------─┐   |     ┌----------------────┐   |
│ close(pipe_fd[0])│  |     │ close(pipe_fd[1])  │   |
│ dup2(infile, 0)  │  |     │ dup2(pipe_fd[0], 0)│   |
│ dup2(pipe_fd[1],1)│ |     │ dup2(outfile, 1)   │   |
│ execve(cmd1)     │  |     │ execve(cmd2)       │   |
└----------------──┘  |     └----------------────┘   |
                      |                               |
                 Parent closes both ends of pipe      |
                 waitpid(child1_pid)                  |
                 waitpid(child2_pid)                  |
---------------------------------------------------
*/

#include <string.h>

//main process, excution function
int main(int ac, char **av, char **envp)
{
    // Check argument count
    if (ac != 5 || strcmp((const char)"pipex", (const char)av[0]) == 1)
        return (0);

    // Open files

    // Create pipe: check path and environments

    // --- First fork: cmd1 ---
        // In child1: grep hello
        // stdin ← infile
        // stdout → pipe write
        // Close unused read end
        // av[2] = "grep hello"

    // --- Second fork: cmd2 ---
        // In child2: wc -l
        // stdin ← pipe read
        // stdout → outfile
        // Close unused write end
        // av[3] = "wc -l"

    // Parent process
    return (0);
}

