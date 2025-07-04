/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuwu <yuwu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 19:31:06 by yuwu              #+#    #+#             */
/*   Updated: 2025/06/26 17:11:03 by yuwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
#include <unistd.h> //write, open, access, dup, dup2, execve, fork, pipe
#include <stdlib.h> //malloc, free, exit
#include <fcntl.h> //open
#include <stdio.h> //write, perror
#include <string.h> //strerror
# include <errno.h> //errno?
# include <sys/wait.h> //WIFEXITED(status)
# include "./libft/libft.h"
*/

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdio.h>
# include <sys/wait.h>
# include "./libft/libft.h"

//cmd.c 
//find paths, find cmd, and execute cmds, and return the exit code to main
//prechecking command existence and return signals of (0/1)
//cleaning up: close, free, perror and exit with code
int		exe_cmd(char *cmd_line, char **envp);
int		check_command_existence(char *cmd_line, char **envp);
void	close_and_error(int *fds, int ppfd[2], const char *msg, int exit_code);

//main.c
//main process, excution function: building pipe, call fk and clean up;
pid_t	ft_fork(int input_fd, int output_fd, char *cmd, char **envp);
void	execute_pipeline(char **av, char **envp, int *wait_status, int *fds);
int		main(int ac, char **av, char **envp);
#endif
