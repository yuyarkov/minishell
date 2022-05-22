/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 15:53:52 by fdarkhaw          #+#    #+#             */
/*   Updated: 2022/05/22 21:24:50 by fdarkhaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*export_create_elem(char *str)
{
	t_env	*new_elem;

	new_elem = malloc(sizeof(t_env));
	if_value_is_null(new_elem);
	new_elem->next = NULL;
	if (ft_strchr(str, '='))
		if_there_is_equal_sign(str, &new_elem);
	else
	{
		new_elem->key = ft_substr(str, 0, ft_strlen(str));
		if_value_is_null(new_elem->key);
		new_elem->value = NULL;
	}
	return (new_elem);
}

int	add_arguments(t_list *cmd, t_env **env)
{
	int		iterator;
	int		result;
	t_env	*new_elem;

	result = 0;
	iterator = 1;
	while (cmd->arguments[iterator])
	{
		if (ft_isalpha(cmd->arguments[iterator][0]) \
			|| cmd->arguments[iterator][0] == '_')
		{
			if (!find_argument_in_env(cmd->arguments[iterator], env))
				new_elem = if_arg_is_not_in_env(cmd->arguments[iterator], env);
		}
		else
		{
			result = 1;
			ft_putstr_fd("minishell: export: '", 2);
			ft_putstr_fd(cmd->arguments[iterator], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
		}
		iterator++;
	}
	return (result);
}

t_env	*find_next_min(t_env *env, t_env *list)
{
	t_env	*strmin;
	t_env	*tmp;

	strmin = ft_calloc(1, sizeof(t_env));
	strmin->key = ft_substr("zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz", 0, 40);
	if_value_is_null(strmin->key);
	strmin->value = ft_calloc(1, sizeof(char));
	tmp = env;
	while (tmp)
	{
		if (find_key_in_list(tmp->key, list))
			tmp = tmp->next;
		else if (strcmp(strmin->key, tmp->key) > 0)
		{
			change_strmin(&strmin, tmp);
			tmp = tmp->next;
		}
		else
			tmp = tmp->next;
	}
	return (strmin);
}

t_env	*create_sort_env(t_env *env)
{
	t_env	*list;
	t_env	*tmp;
	t_env	*min_elem;

	list = ft_calloc(1, sizeof(t_env));
	tmp = env;
	while (tmp)
	{
		min_elem = find_next_min(env, list);
		env_lstadd_back(&list, min_elem);
		tmp = tmp->next;
	}
	return (list);
}

int	execute_export_command(t_list *cmd, char **envp, t_env *env)
{
	int		result;
	t_env	*tmp;

	(void)envp;
	result = 0;
	if (cmd->arguments[1])
		result = add_arguments(cmd, &env);
	else
	{
		tmp = create_sort_env(env);
		print_export(tmp);
		lstiter_env(tmp, free);
	}
	return (result);
}
