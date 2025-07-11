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

void	error_msg(char *str1, char *str2, char *str3)
{
	if (str1)
		ft_putstr_fd(str1, 2);
	if (str2)
		ft_putstr_fd(str2, 2);
	if (str3)
		ft_putstr_fd(str3, 2);
}

void	close_and_error(int *fds, const char *msg, int exit_code)
{
	if (fds)
	{
		if (fds[0] != -1)
			close(fds[0]);
		if (fds[1] != -1)
			close(fds[1]);
		free(fds);
	}
	perror(msg);
	exit(exit_code);
}

void	close_pair(int fd1, int fd2)
{
	if (fd1 >= 0)
		close(fd1);
	if (fd2 >= 0)
		close(fd2);
}

char	**ft_free_split(char **split)
{
	size_t	i;

	if (!split)
		return (NULL);
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
	return (NULL);
}
