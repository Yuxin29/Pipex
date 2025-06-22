/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuwu <yuwu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:36:57 by yuwu              #+#    #+#             */
/*   Updated: 2025/04/17 17:24:14 by yuwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_atoi(const char *nptr)
{
	int	output;
	int	sign;

	output = 0;
	sign = 1;
	while (*nptr == ' ' || (*nptr >= '\t' && *nptr <= '\r'))
		nptr++;
	if ((*nptr == '-') || (*nptr == '+'))
	{
		if (*nptr == '-')
			sign = -1;
		nptr++;
	}
	while (('0' <= *nptr) && (*nptr <= '9'))
	{
		output = output * 10 + (*nptr - '0');
		nptr++;
	}
	return (output * sign);
}
