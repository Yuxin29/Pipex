/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuwu <yuwu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 19:46:51 by yuwu              #+#    #+#             */
/*   Updated: 2025/06/23 20:26:59 by yuwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* 
-------------------------get and execute cmds from the envp---------------------
- perror - print a system error message: void perror(const char *s);
  QQQ: does the subject have specification of "ERROR MESSAGES"?
- for the cmd: I dont need to hard code it, they are ready from shell
*/

#include "pipex.h"

// Find "PATH=..." from envp
// and then skip "PATH=" and return the path string
// this is not a static, it is going to be called in every cmd
// returns a pointer inside envp, not dynamically allocated — no need to free
static char	*find_path_in_envp(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

//Searches for the actual binary file of cmd from the PATH= string.
//esim. inpout	//- cmd = "grep"
		//- path(could be) = "/usr/local/bin:/usr/bin:/bin"
//esim. output	//- "/bin/grep"
//access(one_path, X_OK) == 0: check executability,  0 is yes
static char	*find_command_in_path(char *cmd, char **envp)
{
	char	*path;
	char	**paths;
	char	*temp;
	char	*one_path;
	int		i;

	path = find_path_in_envp(envp);
	if (!path)
		return (write(2, "pipex: no PATH found in environment\n", 34), NULL);
	paths = ft_split(path, ':');
	if (!paths)
		return (write(2, "pipex: error splitting PATH\n", 28), NULL);
	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		one_path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(one_path, X_OK) == 0)
			return (ft_free_split(paths), one_path);
		free(one_path);
		i++;
	}
	return (ft_free_split(paths), NULL);
}

// execution;
// exit code involved here.
// first try yo execute a relative or absolute path
// Error Management
// I try to do it so, if there are null in the 2 static above.
// it only exit from here
void	exe_cmd(char *command_str, char **envp)
{
	char	**cmd_line;
	char	*cmd_path;

	if (!command_str || !*command_str)
		close_and_error(0, 0, "pipex: command not found\n", 127);
	cmd_line = ft_split(command_str, ' ');
	if (!cmd_line || !cmd_line[0] || !*cmd_line[0])
		close_and_error(0, 0, "pipex: command not found\n", 127);
	if (ft_strchr(cmd_line[0], '/'))
	{
		if (access(cmd_line[0], X_OK) == 0)
			execve(cmd_line[0], cmd_line, envp);
		ft_free_split(cmd_line);
		if (errno == ENOENT)
			close_and_error(0, 0, "pipex: command not found\n", 127);
		close_and_error(0, 0, "pipex: command not found\n", 126);
	}
	cmd_path = find_command_in_path(cmd_line[0], envp);
	if (!cmd_path)
	{
		ft_free_split(cmd_line);
		close_and_error(0, 0, "pipex: command not found\n", 127);
	}
	execve(cmd_path, cmd_line, envp);
	ft_free_split(cmd_line);
	if (errno == ENOENT)
		close_and_error(0, 0, "pipex: command not found\n", 127);
	close_and_error(0, 0, "pipex: command not found\n", 126);
}
