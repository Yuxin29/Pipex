/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuwu <yuwu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 19:46:51 by yuwu              #+#    #+#             */
/*   Updated: 2025/06/25 18:00:27 by yuwu             ###   ########.fr       */
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
// first try yo execute a relative or absolute path
// About Error Management
// I try to do it so, if there are null in the 2 static above, 
// it only give error signals here but not exit, pass the error sig int to main
// because in the main, there might be reacheable mem of fds if exit here
int	exe_cmd(char *cmd, char **envp)
{
	char	**args;
	char	*path;

	args = ft_split(cmd, ' ');
	if (!args)
		return (1);
	if (ft_strchr(args[0], '/'))
	{
		execve(args[0], args, envp);
		ft_putstr_fd("pipex: ", 2);
		perror(args[0]);
		return (ft_free_split(args), 126);
	}
	path = find_command_in_path(args[0], envp);
	if (!path)
	{
		ft_putstr_fd("pipex: command not found: ", 2);
		ft_putendl_fd(args[0], 2);
		return (ft_free_split(args), 127);
	}
	execve(path, args, envp);
	free(path);
	ft_free_split(args);
	return (126);
}
