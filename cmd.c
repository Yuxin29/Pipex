/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuwu <yuwu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 19:46:51 by yuwu              #+#    #+#             */
/*   Updated: 2025/06/22 18:58:21 by yuwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* 
-------------------------get and execute cmds-----------------------------

- perror - print a system error message: void perror(const char *s);
    HUOM: does the subject have specification of "ERROR MESSAGES"
HUOM
- for the cmd: I dont need to hard code it, they are ready from shell
*/

#include "pipex.h"

// Find "PATH=..." from envp
// this is not a static, it is going to be called in every cmd/
// skip "PATH=" and return the path string
//returns a pointer inside envp, not dynamically allocated — no need to free
char	*find_path_in_envp(char **envp)
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
char	*find_command_in_path(char *cmd, char **envp)
{
	char	*path;
	char	**paths;
	char	*temp;
	char	*one_path;
	int		i;

	path = find_path_in_envp(envp);
	if (!path)
		return (write(2, "pipex: no PATH variable found in environment\n", 43), NULL);
	paths = ft_split(path, ':');
	if (!paths)
	{
		write(2, "pipex: error splitting PATH\n", 27);
		exit(1);
	}
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

static void	exit_command_not_found(char **cmd_line)
{
	ft_putstr_fd("pipex: ", 2);
	ft_putstr_fd(cmd_line[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	ft_free_split(cmd_line);
	exit(127);
}

void	exe_cmd(char *command_str, char **envp)
{
	char	**cmd_line;
	char	*cmd_path;

	if (!command_str || !*command_str)
		exit(126); // Permission denied
	cmd_line = ft_split(command_str, ' ');
	if (!cmd_line || !cmd_line[0])
		exit(127); // Command not found
	if (ft_strchr(cmd_line[0], '/')) // Handle absolute/relative paths
	{
		if (access(cmd_line[0], X_OK) == 0)
		{
			execve(cmd_line[0], cmd_line, envp); // If execve returns, it failed
			perror("pipex");
			ft_free_split(cmd_line);
			exit(126); // Permission denied
		}
		exit_command_not_found(cmd_line);
	}
	cmd_path = find_command_in_path(cmd_line[0], envp);
	if (!cmd_path)
		exit_command_not_found(cmd_line);
	execve(cmd_path, cmd_line, envp);
	perror("pipex");
	free(cmd_path);
	ft_free_split(cmd_line);
	exit(126);
}
