/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/20 18:37:19 by jg                #+#    #+#             */
/*   Updated: 2022/04/16 18:32:11 by dirony           ###   ########.fr       */
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
		if (iter->cmd)
			free(iter->cmd);
		if (iter->arguments)
			free_string_array(iter->arguments);
		temp = iter;
		iter = iter->next;
		free(temp);		
	}
}

void	clear_tokens(t_info *info)
{
	int	i;

	i = 0;
	while (i < info->num_of_tokens)
	{
		free(info->tokens[i].value);
		i++;
	}
	free(info->tokens);
}