/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuwu <yuwu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 21:18:22 by yuwu              #+#    #+#             */
/*   Updated: 2025/05/02 16:25:39 by yuwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	s;
	size_t	i;
	size_t	len_little;

	if (!(*little))
		return ((char *)big);
	s = 0;
	len_little = ft_strlen(little);
	while ((*big) && (s + len_little <= len))
	{
		if (*little == *big)
		{
			i = 0;
			while ((little[i]) && (little[i] == big[i]))
				i++;
			if (!(little[i]))
				return ((char *)(big));
		}
		s++;
		big++;
	}
	return (NULL);
}
