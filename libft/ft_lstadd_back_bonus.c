/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/09 20:32:37 by dirony            #+#    #+#             */
/*   Updated: 2021/10/15 20:03:51 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **list, t_list *new_elem)
{
	t_list	*iterator;

	iterator = *list;
	if (*list)
	{
		while (iterator->next)
			iterator = iterator->next;
		iterator->next = new_elem;
	}
	else
		*list = new_elem;
}
