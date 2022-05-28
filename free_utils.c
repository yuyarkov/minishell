/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/20 18:37:19 by jg                #+#    #+#             */
/*   Updated: 2022/05/28 16:22:03 by fdarkhaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_befor_exit(char *str, char **split_arg, t_info *info)
{
	free_string_array(split_arg);
	free(str);
	clear_info(info);
}

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

void	clear_info(t_info *info)
{
	clear_info_except_envp(info);
	lstiter_env(info->env, free);
	if (info->envp && info->changed_envp)
		free_string_array(info->envp);
}
