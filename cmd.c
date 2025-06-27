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
	char	*paths;
	char	**path;
	char	*temp;
	char	*one_path;
	int		i;

	paths = find_path_in_envp(envp);
	if (!paths)
		return (write(2, "pipex: no PATH found in environment\n", 34), NULL);
	path = ft_split(paths, ':');
	if (!path)
		return (write(2, "pipex: error splitting PATH\n", 28), NULL);
	i = 0;
	while (path[i])
	{
		temp = ft_strjoin(path[i], "/");
		one_path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(one_path, X_OK) == 0)
			return (ft_free_split(path), one_path);
		free(one_path);
		i++;
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

	args = ft_split(cmd_line, ' ');
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

void	close_and_error(int *fds, int ppfd[2], const char *msg, int exit_code)
{
	if (fds)
	{
		if (fds[0] >= 0)
			close(fds[0]);
		if (fds[1] >= 0)
			close(fds[1]);
		free(fds);
	}
	if (ppfd)
	{
		if (ppfd[0] >= 0)
			close(ppfd[0]);
		if (ppfd[1] >= 0)
			close(ppfd[1]);
	}
	if (msg)
		perror(msg);
	exit(exit_code);
}

//this one is going to be called in the main
//prechecking cmd existence and return a in as signal
//1 as existing and 0 as non_existing
int	check_command_existence(char *cmd_line, char **envp)
{
	char	**args;
	char	*path;
	int		existence;

	existence = 0;
	args = ft_split(cmd_line, ' ');
	if (!args)
		return (1);
	if (ft_strchr(args[0], '/'))
	{
		if (access(args[0], X_OK) == 0)
			existence = 1;
	}
	else
	{
		path = find_command_in_path(args[0], envp);
		if (path)
			existence = 1;
		free(path);
	}
	ft_free_split(args);
	return (existence);
}
