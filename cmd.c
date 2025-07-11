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
#include <string.h>

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
	char	*paths = NULL;
	char	**path = NULL;
	char	*one_path;
	int		j;

	j = 0;
	while (envp[j])
	{
		if (strncmp(envp[j], "PATH=", 5) == 0)
		{
			paths = envp[j] + 5;
			break ;
		}
		j++;
	}
	if (!paths)
		return (NULL);
	path = ft_split(paths, ':');
	if (!path)
		return (NULL);
	j = 0;
	while (path[j])
	{
		one_path = safe_join(path[j], cmd);
		if (!one_path)
			return (ft_free_split(path), NULL);
		if (access(one_path, X_OK) == 0)
			return (ft_free_split(path), one_path);
		free(one_path);
		j++;
	}
	ft_free_split(path);
	return (NULL);
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

	if (!cmd_line || !*cmd_line)
		return (ft_putstr_fd("pipex: command not found\n", 2), 127);
	args = ft_split(cmd_line, ' ');
	if (!args || !args[0])
	{
		ft_free_split(args);
		return (ft_putstr_fd("pipex: command not found\n", 2), 127);
	}
	if (ft_strchr(args[0], '/'))
		path = ft_strdup(args[0]);
	else
		path = find_command_in_path(args[0], envp);
	if (!path)
	{
		error_msg("pipex: ", args[0], ": command not found\n");
		return (ft_free_split(args), 127);
	}
	execve(path, args, envp);
    ft_free_split(args);
    free(path);
    if (errno == ENOENT)
        return (error_msg("pipex: ", args[0], ": No such file or directory\n"), 127);
    else if (errno == EACCES)
		return (error_msg("pipex: ", args[0], ": Permission denied\n"), 126);
    return (error_msg("pipex: ", args[0], ": Permission denied\n"), 1);
}