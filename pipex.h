/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuwu <yuwu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 19:31:06 by yuwu              #+#    #+#             */
/*   Updated: 2025/06/28 11:27:14 by yuwu             ###   ########.fr       */
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
*/

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <errno.h>
# include "./libft/libft.h"

# define ERR_SPLIT "pipex: split failed\n"

//helper.c 
//cleaning up: close, free, perror and exit with code
void	error_126(int st, char *str);
void	error_msg(char *str1, char *str2, char *str3);
void	close_and_error(int *fds, int ppfd[2], char *msg, int exit_code);
void	close_pair(int fd1, int fd2);
void	free_split(char **split);

//cmd.c 
//find paths, find cmd, and execute cmds, and return the exit code to main
//prechecking command existence and return signals of (1 / 126 / 127)
//error msgs here: empty cmd, split failure and execution failure
int		exe_cmd(char *cmd_line, char **envp);
int		check_command_existence(char *cmd_line, char **envp);

//main.c
//main process, excution function: building pipe, call fk;
//void	execute_pipeline(char **av, char **envp, int *wait_status, int *fds);
int		main(int ac, char **av, char **envp);
#endif
