/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/09 22:27:01 by dirony            #+#    #+#             */
/*   Updated: 2021/10/15 19:57:21 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *list, void *(*f)(void *), void (*del)(void *))
{
	t_list	*result;
	t_list	*new_elem;
	t_list	*iterator;

	if (!list || !f)
		return (NULL);
	result = NULL;
	iterator = list;
	while (iterator)
	{
		new_elem = ft_lstnew(f(iterator->content));
		if (NULL == new_elem)
		{
			ft_lstclear(&result, del);
			return (NULL);
		}
		ft_lstadd_back(&result, new_elem);
		iterator = iterator->next;
	}
	return (result);
}
