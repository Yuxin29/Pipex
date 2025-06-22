/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuwu <yuwu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 15:58:55 by yuwu              #+#    #+#             */
/*   Updated: 2025/04/19 16:20:12 by yuwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	total;
	void	*dest;

	if ((nmemb) && (size > SIZE_MAX / nmemb))
		return (NULL);
	total = nmemb * size;
	dest = (void *)malloc(total);
	if (!(dest))
		return (NULL);
	ft_bzero(dest, total);
	return (dest);
}
