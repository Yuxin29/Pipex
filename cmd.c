/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuwu <yuwu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 19:46:51 by yuwu              #+#    #+#             */
/*   Updated: 2025/06/19 19:46:56 by yuwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//get and execute cmds: cmd.c
char **make_argv_array(char *command_str);
char *find_path_in_envp(char **envp); 
char *find_command_in_path(char *cmd, char *path);
void exe_cmd1(input, cmd1, output);
void exe_cmd1(input, cmd2, output);
