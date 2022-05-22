/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/20 18:37:19 by jg                #+#    #+#             */
/*   Updated: 2022/05/22 21:30:17 by fdarkhaw         ###   ########.fr       */
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
	while (info->tokens && info->tokens[i].type != END_OF_TOKENS)
	{
		free(info->tokens[i].value);
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

void	clear_info_except_envp(t_info *info)
{
	t_list	*iter;
	t_list	*temp;

	clear_tokens(info);
	if (info->envp)
		free_string_array(info->envp);
	// if (info->limiters)//тут происходила лишняя очистка, т.к. структура не была обнулена
	// 	free(info->limiters);
	if (info->commands)
	{
		iter = info->commands;
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
	info->commands = NULL;
}

void	free_after_ctrl_d(char *str, t_info *info)
{
	free(str);
	clear_info(info);
	ft_putstr_fd("\x1b[1F", 1);
	ft_putstr_fd(SHELL, 1);
	ft_putendl_fd("exit", 1);
	//clear_info_except_envp(info);
	free(str);//зачем фришить str дважды?
}
