/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuwu <yuwu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 12:03:18 by yuwu              #+#    #+#             */
/*   Updated: 2025/04/27 12:21:08 by yuwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*r_value;

	r_value = malloc(sizeof(t_list) * 1);
	if (!r_value)
		return (NULL);
	r_value->content = content;
	r_value->next = NULL;
	return (r_value);
}
