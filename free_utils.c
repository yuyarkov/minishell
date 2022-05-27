/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/20 18:37:19 by jg                #+#    #+#             */
/*   Updated: 2022/05/27 21:48:56 by fdarkhaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_string_array(char **str)
{
	int	i;

	i = 0;
	while (str && str[i])
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
	while (info->tokens && info->tokens[i].type != EOF_TOKENS)
	{
		free(info->tokens[i].value);
		info->tokens[i].value = NULL;
		i++;
	}
	if (info->tokens)
		free(info->tokens);
	info->tokens = NULL;
}

void	clear_info(t_info *info)//зачаток общей функции, которая чистит всё
{
	clear_info_except_envp(info);
	lstiter_env(info->env, free);//освобождаю сам связный список и его поля
	if (info->envp && info->changed_envp)
		free_string_array(info->envp);
}

void	set_values_to_null(t_list *iter)
{
	iter->cmd = NULL;
	iter->arguments = NULL;
	iter->redirect_in_file = NULL;
	iter->out_file = NULL;
}

void	clear_cmd(t_list *iter)
{
	t_list	*temp;
	
	while (iter)
	{
		if (iter->cmd)
			free(iter->cmd);
		if (iter->arguments)
			free_string_array(iter->arguments);
		if (iter->redirect_in_file)
			free(iter->redirect_in_file);
		if (iter->out_file)
			free(iter->out_file);
		set_values_to_null(iter);
		temp = iter;
		iter = iter->next;
		free(temp);
	}
}

void	clear_info_except_envp(t_info *info)
{
	// t_list	*iter;
	// t_list	*temp;

	clear_tokens(info);
	if (info->envp)
		free_string_array(info->envp);
	// if (info->commands)
	// {
	// 	iter = info->commands;
	// 	while (iter)
	// 	{
	// 		if (iter->cmd)
	// 			free(iter->cmd);
	// 		if (iter->arguments)
	// 			free_string_array(iter->arguments);
	// 		if (iter->redirect_in_file)
	// 			free(iter->redirect_in_file);
	// 		if (iter->out_file)
	// 			free(iter->out_file);
	// 		set_values_to_null(iter);
	// 		temp = iter;
	// 		iter = iter->next;
	// 		free(temp);
	// 	}
	// }
	info->commands = NULL;
}
