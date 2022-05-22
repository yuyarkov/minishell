/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/22 15:35:11 by fdarkhaw          #+#    #+#             */
/*   Updated: 2022/05/22 21:29:28 by fdarkhaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_key_in_list(char *argument, t_env *list)
{
	int		a_in_l;

	while (list)
	{
		a_in_l = ft_strcmp(argument, list->key);
		if (!a_in_l)
			return (1);
		list = list->next;
	}
	return (0);
}

void	print_export(t_env *env)
{
	while (env)
	{
		if (env->value && env->key)
		{
			ft_putstr_fd("declare -x ", 1);
			ft_putstr_fd(env->key, 1);
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(env->value, 1);
			ft_putendl_fd("\"", 1);
		}
		else if (!env->value && env->key)
		{
			ft_putstr_fd("declare -x ", 1);
			ft_putendl_fd(env->key, 1);
		}
		env = env->next;
	}
}

void	change_strmin(t_env **strmin, t_env *tmp)
{
	free((*strmin)->key);
	(*strmin)->key = ft_substr(tmp->key, 0, ft_strlen(tmp->key));
	if_value_is_null((*strmin)->key);
	if (tmp->value)
	{
		free((*strmin)->value);
		(*strmin)->value = ft_substr(tmp->value, 0, ft_strlen(tmp->value));
		if_value_is_null((*strmin)->value);
	}
	else
		(*strmin)->value = NULL;
}
