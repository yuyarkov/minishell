/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/20 18:37:19 by jg                #+#    #+#             */
/*   Updated: 2022/04/08 21:00:06 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_string_array(char **str)
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
		if (list->key)
			f(list->key);
		if (list->value)
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

void	clear_list_env(t_env *env)//зачаток общей функции, которая чистит всё
{
	t_env	*iter;
	t_env	*temp;
	
	iter = env;
	while (iter)
	{
		temp = iter;
		iter = iter->next;
		free(temp);		
	}
}

void	clear_info(t_info *info, t_list *commands)//зачаток общей функции, которая чистит всё
{
	t_list	*iter;
	t_list	*temp;
	
	if (info->limiters)
		free(info->limiters);
	iter = commands;
	while (iter)
	{
		free(iter->cmd);
		free_string_array(iter->arguments);
		temp = iter;
		iter = iter->next;
		free(temp);		
	}
}

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