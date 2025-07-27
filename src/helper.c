/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuwu <yuwu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 11:19:23 by yuwu              #+#    #+#             */
/*   Updated: 2025/06/28 12:01:14 by yuwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

//it gives error message of 126 and 127
void	error_126(int st, char *str)
{
	if (st == 126)
		error_msg("pipex: ", str, ": Permission denied\n");
	else if (st == 127 && (!str || *str == '\0'))
		ft_putstr_fd("pipex: command not found\n", 2);
	else if (st == 127)
	{
		if (ft_strchr(str, '/'))
			error_msg(str, NULL, ": No such file or directory\n");
		else
			error_msg("pipex: ", str, ": command not found\n");
	}
}

void	error_msg(char *str1, char *str2, char *str3)
{
	if (str1)
		ft_putstr_fd(str1, 2);
	if (str2)
		ft_putstr_fd(str2, 2);
	if (str3)
		ft_putstr_fd(str3, 2);
}

void	close_and_error(int *fds, int ppfd[2], char *msg, int exit_code)
{
	if (fds)
		close_pair(fds[0], fds[1]);
	if (ppfd)
		close_pair(ppfd[0], ppfd[1]);
	if (msg)
		ft_putstr_fd(msg, 2);
	exit(exit_code);
}

void	close_pair(int fd1, int fd2)
{
	if (fd1 >= 0)
	{
		close(fd1);
		fd1 = -1;
	}
	if (fd2 >= 0)
	{
		close(fd2);
		fd2 = -1;
	}
}

//defensicve closing
void	close_fds_from(int start)
{
	int	i;

	i = start;
	while (i < 1024)
	{
		close(i);
		i++;
	}
}
