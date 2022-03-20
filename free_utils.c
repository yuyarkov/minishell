/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jg <jg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/20 18:37:19 by jg                #+#    #+#             */
/*   Updated: 2022/03/20 18:38:35 by jg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_str_pointer(char **str)
{
	int	iterator;

	iterator = 0;
	while (str[iterator])
	{
		free(str[iterator]);
		iterator++;
	}
	free(str);
}

void	lstiter_env(t_env *list, void (*f)(void *))
{
	while (list)
	{
		f(list->key);
		f(list->value);
		list = list->next;
	}
}

void	clear_env(t_env *list)
{
	if (list)
	{
		clear_env(list->next);
		free(list);
		list = NULL;
	}
}

// void	ft_lstclear(t_list **list, void (*del)(void *))
// {
// 	t_list	*temp;

// 	while (*list)
// 	{
// 		temp = (*list)->next;
// 		ft_lstdelone(*list, del);
// 		*list = temp;
// 	}
// 	*list = NULL;
// }

// void	ft_lstadd_front(t_list **list, t_list *new_elem)
// {
// 	if (*list)
// 	{
// 		new_elem->next = *list;
// 		*list = new_elem;
// 	}
// 	else
// 		*list = new_elem;
// }