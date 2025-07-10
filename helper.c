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

int	send_error_msg(const char *msg)
{
	if (msg)
		perror(msg);
	if (errno == EACCES)
		ft_putendl_fd("Permission denied", 2);
	return (1);
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

void	close_all(int	*ppfd, int	*fds)
{
	if (ppfd[0] >= 0)
		close(ppfd[0]);
	if (ppfd[1] >= 0)
		close(ppfd[1]);
	if (fds[0] >= 0)
		close(fds[0]);
	if (fds[1] >= 0)
		close(fds[1]);
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
