/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuwu <yuwu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 15:17:44 by yuwu              #+#    #+#             */
/*   Updated: 2025/06/22 22:03:12 by yuwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>  //write, open, access, dup, dup2, execve, fork, pipe
# include <stdlib.h>  //malloc, free, exit
# include <fcntl.h>  //open
# include <stdio.h>  //write, perror
# include <string.h> //strerror
# include <errno.h> //errno?
# include <sys/wait.h> //wait, waitpid, ??fork as well??
# include "./libft/libft.h"

//io.c
//get input and output fds
//Error exit and closeup fds
int		handle_exit_status(int status1, int status2);
int		open_file(const char *filename, int is_output);
void	ft_error(char *error_msg);
void	close_all_four(int fd1, int fd2, int ppfd[2]);

//cmd.c 
//find paths from envoronments, find cmd in paths, and execute cmds
char	*find_path_in_envp(char **envp);
char	*find_command_in_path(char *cmd, char **envp);
void	exe_cmd(char *command_str, char **envp);

//main.c
//main process, excution function: building pipe, call fk and clean up;
// 2 static fork ft and 1 static pipelien and 1 statuc ini io inside here.
int		main(int ac, char **av, char **envp);

#endif
