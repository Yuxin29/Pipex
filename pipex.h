/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuwu <yuwu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 21:01:10 by yuwu              #+#    #+#             */
/*   Updated: 2025/06/23 22:17:09 by yuwu             ###   ########.fr       */
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

//cmd.c 
//find paths from envoronments, find cmd in paths, and execute cmds
//static char	*find_path_in_envp(char **envp);
//static char	*find_command_in_path(char *cmd, char **envp);
void	exe_cmd(char *command_str, char **envp);

//main.c
//main process, excution function: building pipe, call fk and clean up;
void	close_and_error(int *fds, int ppfd[2], const char *msg, int exit_code);
pid_t	ft_fork(int input_fd, int output_fd, char *cmd, char **envp);
int		*init_fds(char **av);
void	execute_pipeline(char **av, char **envp, int *status1, int *status2);
int		main(int ac, char **av, char **envp);

#endif
