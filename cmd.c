/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuwu <yuwu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 19:46:51 by yuwu              #+#    #+#             */
/*   Updated: 2025/06/28 12:23:03 by yuwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* 
-------------------------get and execute cmds from the envp---------------------
- perror - print a system error message: void perror(const char *s);
- for the cmd: I dont need to hard code it, they are ready from shell
------close and free fds & pipefd_send error msg and exit with code-------------
------------precheck_command_existence and return int as signals----------------
*/

#include "pipex.h"

// Find "PATH=..." from environmental ptr
// and then skip "PATH=" and return the path string
static char	*find_path_in_envp(char **envp)
{
	int	i;

	i = 0;
	if (!envp)
		return (NULL);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

static char	*safe_join(const char *path, const char *cmd)
{
	char	*temp;
	char	*result;

	if (!path || !cmd)
		return (NULL);
	temp = ft_strjoin(path, "/");
	if (!temp)
		return (NULL);
	result = ft_strjoin(temp, cmd);
	free (temp);
	if (!result)
		free(temp);
	return (result);
}

//Searches for the actual binary file of cmd from the PATH= string.
//esim. inpout	//- cmd = "grep"
//- path(could be) = "/usr/local/bin:/usr/bin:/bin"
//esim. output	//- "/bin/grep"
//access(one_path, X_OK) == 0: check executability,  0 is yes
static char	*find_command_in_path(char *cmd, char **envp)
{
	char	*paths;
	char	**path;
	char	*one_path;
	int		i;

	if (ft_strchr(cmd, '/') && access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	paths = find_path_in_envp(envp);
	if (!paths)
		return (NULL);
	path = ft_split(paths, ':');
	if (!path)
		return (NULL);
	i = 0;
	while (path[i])
	{
		one_path = safe_join(path[i], cmd);
		if (!one_path)
			return (ft_free_split(path), NULL);
		if (access(one_path, X_OK) == 0)
			return (ft_free_split(path), one_path);
		free(one_path);
		i++;
	}
	return (ft_free_split(path), NULL);
}

// execution;
// first try to execute a relative or absolute path
// About Error Management
// I try to do it so, if there are null in the 2 static above, 
// it only give error signals here but not exit, pass the exit code to main
// because in the main, there might be reacheable mem of fds if exit here
int	exe_cmd(char *cmd_line, char **envp)
{
	char	**args;
	char	*path;
	int		err;

	if (!cmd_line || !*cmd_line)
		return (ft_putstr_fd("pipex: command not found\n", 2), 127);
	args = ft_split(cmd_line, ' ');
	if (!args || !args[0])
		return (ft_free_split(args), ft_putstr_fd("pipex: command not found\n", 2), 127);
	if (ft_strchr(args[0], '/'))
	{
		if (access(args[0], F_OK) == -1)
		{
			ft_free_split(args);
			return (error_msg("pipex: ", args[0], ": No such file or directory\n"), 127);
		}
		if (access(args[0], X_OK) == -1)
		{
			ft_free_split(args);
			return (error_msg("pipex: ", args[0], ": Permission denied\n"), 126);
		}
		execve(args[0], args, envp);
		err = errno;
		ft_free_split(args);
		if (err == EACCES)
			return (error_msg("pipex: ", args[0], ": Permission denied\n"), 126);
		return (error_msg("pipex: ", args[0], ": execution failed\n"), 1);
	}
	path = find_command_in_path(args[0], envp);
	if (!path)
		return (ft_free_split(args), error_msg("pipex: ", args[0], ": command not found\n"), 127);
	execve(path, args, envp);
	err = errno;
	free(path);
	ft_free_split(args);
	if (err == ENOENT)
		return (error_msg("pipex: ", args[0], ": command not found\n"), 127);
	if (err == EACCES)
		return (error_msg("pipex: ", cmd_line, ": Permission denied\n"), 126);
	return (error_msg("pipex: ", cmd_line, ": execution failed\n"), 1);
}

//this one is going to be called in the main
//prechecking cmd existence and return a in as signal
//1 as existing and 0 as non_existing
int	check_command_existence(char *cmd_line, char **envp)
{
	char	**args;
	char	*path;
	int		result;

	if (!cmd_line || !*cmd_line)
		return (127);
	while (*cmd_line == ' ' || *cmd_line == '\t')
		cmd_line++;
	if (!*cmd_line)
		return (127);
	args = ft_split(cmd_line, ' ');
	if (!args || !args[0])
		return (ft_free_split(args), 127);

	result = 127;
	if (ft_strchr(args[0], '/'))
	{
		if (access(args[0], F_OK) == 0)
			result = (access(args[0], X_OK) == 0) ? 1 : 126;
	}
	else
	{
		path = find_command_in_path(args[0], envp);
		if (path)
		{
			result = (access(path, X_OK) == 0) ? 1 : 126;
			free(path);
		}
	}
	ft_free_split(args);
	return (result);
}
