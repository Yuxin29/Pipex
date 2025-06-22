/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuwu <yuwu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 18:40:45 by yuwu              #+#    #+#             */
/*   Updated: 2025/04/14 18:40:49 by yuwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	j;
	size_t	k;

	j = 0;
	k = ft_strlen(src);
	if (size < 1)
		return (k);
	while ((src[j]) && (j + 1 < size))
	{
		dst[j] = src[j];
		j++;
	}
	dst[j] = '\0';
	return (k);
}
