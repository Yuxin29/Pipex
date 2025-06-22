/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuwu <yuwu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 13:50:31 by yuwu              #+#    #+#             */
/*   Updated: 2025/05/03 15:19:49 by yuwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_itoa_positive(int n)
{
	char	reverse[12];
	int		i;
	int		j;
	char	*result;

	i = 0;
	j = 0;
	if (n == 0)
		return (ft_strdup("0"));
	while (n > 0)
	{
		reverse[i] = (n % 10) + '0';
		i++;
		n /= 10;
	}
	result = malloc(i + 1);
	if (!result)
		return (NULL);
	while (j < i)
	{
		result[j] = reverse[i - j - 1];
		j++;
	}
	result[i] = '\0';
	return (result);
}

char	*ft_itoa(int n)
{
	char	*temp;

	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	if (n < 0)
	{
		temp = ft_strjoin("-", ft_itoa_positive(-n));
		if (!temp)
			return (NULL);
		return (temp);
	}
	return (ft_itoa_positive(n));
}
