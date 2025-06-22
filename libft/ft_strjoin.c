/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuwu <yuwu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:10:44 by yuwu              #+#    #+#             */
/*   Updated: 2025/04/19 18:20:25 by yuwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*result;
	size_t	n;

	if (!s1 || !s2)
		return (NULL);
	result = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!(result))
		return (NULL);
	n = 0;
	while (*s1)
	{
		result[n] = *s1;
		n++;
		s1++;
	}
	while (*s2)
	{
		result[n] = *s2;
		n++;
		s2++;
	}
	result[n] = '\0';
	return (result);
}
