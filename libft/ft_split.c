/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuwu <yuwu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 13:24:34 by yuwu              #+#    #+#             */
/*   Updated: 2025/05/01 18:47:07 by yuwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_words(const char *s, char c)
{
	size_t	count;

	count = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			count++;
			while (*s && *s != c)
				s++;
		}
	}
	return (count);
}

static char	*copy_word(const char *s, char c)
{
	char	*word;
	size_t	i;
	size_t	len;

	len = 0;
	while (s[len] && s[len] != c)
		len++;
	word = malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i] = s[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

static char	**free_split(char **split, size_t filled)
{
	size_t	i;

	i = 0;
	while (i < filled)
	{
		free(split[i]);
		i++;
	}
	return (free(split), split = NULL, NULL);
}

char	**ft_split(char const *s, char c)
{
	char		**result;
	size_t		index;

	index = 0;
	if (!s)
		return (NULL);
	result = malloc(sizeof(char *) * (count_words(s, c) + 1));
	if (!result)
		return (NULL);
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			result[index] = copy_word(s, c);
			if (!result[index])
				return (free_split(result, index));
			s += ft_strlen(result[index]);
			index++;
		}
	}
	result[index] = NULL;
	return (result);
}
