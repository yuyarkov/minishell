/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/20 18:37:19 by jg                #+#    #+#             */
/*   Updated: 2022/05/15 18:39:15 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_string_array(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	if (str)
		free(str);
}

void	lstiter_env(t_env *list, void (*f)(void *))
{
	t_env	*temp;

	while (list)
	{
		if (list->key)
			f(list->key);
		if (list->value)
			f(list->value);
		temp = list;
		list = list->next;
		f(temp);
	}
}

void	clear_tokens(t_info *info)
{
	int	i;

	i = 0;
	while (info->tokens && i < info->num_of_tokens)
	{
		free(info->tokens[i].value);
		i++;
	}
	if (info->tokens)
		free(info->tokens);
}

void	clear_info(t_info info)//зачаток общей функции, которая чистит всё
{
	t_list	*iter;
	t_list	*temp;

	if (info.limiters)//тут происходила лишняя очистка, т.к. структура не была обнулена
		free(info.limiters);
	lstiter_env(info.env, free);//освобождаю сам связный список и его поля
	if (info.commands)
	{
		iter = info.commands;
		while (iter)
		{
			if (iter->cmd)
				free(iter->cmd);
			if (iter->arguments)
				free_string_array(iter->arguments);
			if (iter->redirect_in_file)
				free(iter->redirect_in_file);
			if (iter->redirect_out_file)
				free(iter->redirect_out_file);
			temp = iter;
			iter = iter->next;
			free(temp);
		}
	}
	if (info.envp)
		free_string_array(info.envp);
}
