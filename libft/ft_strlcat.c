/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuwu <yuwu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 18:41:01 by yuwu              #+#    #+#             */
/*   Updated: 2025/04/14 18:41:19 by yuwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	j;
	size_t	k;

	if (!dst || !src)
		return (0);
	i = ft_strlen(dst);
	k = ft_strlen(src);
	j = 0;
	if (size <= i)
		return (size + k);
	while ((src[j]) && (j + i + 1 < size))
	{
		dst[i + j] = src[j];
		j++;
	}
	dst[i + j] = '\0';
	return (i + k);
}
