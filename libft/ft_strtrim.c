/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuwu <yuwu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 11:25:05 by yuwu              #+#    #+#             */
/*   Updated: 2025/04/20 11:46:25 by yuwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_belongto_set(char c, char const *set)
{
	while (*set)
	{
		if (c == *set)
			return (1);
		set++;
	}
	return (0);
}

static size_t	ft_trim_start(char const *s1, char const *set)
{
	size_t	start_index;

	start_index = 0;
	while (s1[start_index] && ft_belongto_set(s1[start_index], set) != 0)
		start_index++;
	return (start_index);
}

static size_t	ft_trim_end(char const *s1, char const *set)
{
	size_t	end_index;

	end_index = ft_strlen(s1);
	while (end_index && ft_belongto_set(s1[end_index - 1], set))
		end_index--;
	return (end_index);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	len;
	size_t	s_index;
	size_t	e_index;
	char	*trimed;
	size_t	n;

	if (!s1 || !set)
		return (NULL);
	n = 0;
	s_index = ft_trim_start(s1, set);
	e_index = ft_trim_end(s1, set);
	if (s_index >= e_index)
		len = 0;
	else
		len = e_index - s_index;
	trimed = malloc(sizeof(char) * (len + 1));
	if (!(trimed))
		return (NULL);
	while (n < len)
	{
		trimed[n] = s1[s_index + n];
		n++;
	}
	trimed[n] = '\0';
	return (trimed);
}
