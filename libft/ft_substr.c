/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuwu <yuwu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 19:14:59 by yuwu              #+#    #+#             */
/*   Updated: 2025/04/26 18:28:11 by yuwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub;
	size_t	n;
	size_t	max;

	if (!s)
		return (NULL);
	max = ft_strlen(s);
	if (start >= max)
		return (ft_strdup(""));
	if (start + len <= max)
		sub = malloc(sizeof(char) * (len + 1));
	else
		sub = malloc(sizeof(char) * (max - start + 1));
	if (!(sub))
		return (NULL);
	n = 0;
	while (s[start + n] && n < len)
	{
		sub[n] = s[start + n];
		n++;
	}
	sub[n] = '\0';
	return (sub);
}
