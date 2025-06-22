/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuwu <yuwu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 20:40:19 by yuwu              #+#    #+#             */
/*   Updated: 2025/04/28 19:30:52 by yuwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_putnbr_fd_positive(int n, int fd)
{
	char	c;

	if (n > 9)
		ft_putnbr_fd_positive(n / 10, fd);
	c = (n % 10) + '0';
	ft_putchar_fd(c, fd);
}

void	ft_putnbr_fd(int n, int fd)
{
	if (n == -2147483648)
	{
		write(fd, "-2147483648", 11);
		return ;
	}
	if (n < 0 && n != -2147483648)
	{
		ft_putchar_fd('-', fd);
		n = -n;
	}
	ft_putnbr_fd_positive(n, fd);
}
