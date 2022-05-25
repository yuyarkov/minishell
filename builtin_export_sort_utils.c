/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_sort_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/22 15:35:11 by fdarkhaw          #+#    #+#             */
/*   Updated: 2022/05/24 21:26:12 by fdarkhaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	{
		free((*strmin)->value);
		(*strmin)->value = NULL;
	}
}

int	find_key_in_list(char *argument, t_env *list)
{
	int		arg_in_list;

	while (list)
	{
		arg_in_list = ft_strcmp(argument, list->key);
		if (!arg_in_list)
			return (1);
		list = list->next;
	}
	return (0);
}

t_env	*find_next_min(t_env *env, t_env *list)
{
	t_env	*strmin;
	t_env	*tmp;

	strmin = ft_calloc(1, sizeof(t_env));
	strmin->key = ft_substr("~", 0, 1);
	if_value_is_null(strmin->key);
	strmin->value = ft_calloc(1, sizeof(char));
	tmp = env;
	while (tmp)
	{
		if (find_key_in_list(tmp->key, list))
			tmp = tmp->next;
		else if (ft_strcmp(strmin->key, tmp->key) > 0)
		{
			change_strmin(&strmin, tmp);
			tmp = tmp->next;
		}
		else
			tmp = tmp->next;
	}
	return (strmin);
}

void	create_sort_env(t_env *env)
{
	t_env	*list;
	t_env	*tmp;
	t_env	*min_elem;

	list = ft_calloc(1, sizeof(t_env));
	if_value_is_null(list);
	tmp = env;
	while (tmp)
	{
		min_elem = find_next_min(env, list);
		env_lstadd_back(&list, min_elem);
		tmp = tmp->next;
	}
	print_export(list);
	lstiter_env(list, free);
}
