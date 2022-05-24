/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/19 23:25:51 by fdarkhaw          #+#    #+#             */
/*   Updated: 2022/05/24 20:49:11 by fdarkhaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// t_env	*if_arg_is_not_in_env(char *arg, t_env **env)
// {
// 	t_env	*elem;

// 	elem = env_create_elem(arg);
// 	if_value_is_null(elem);
// 	env_lstadd_back(env, elem);
// 	return (elem);
// }

void	if_there_is_equal_sign(char *str, t_env **new_elem)
{
	char	**result;

	result = ft_split(str, '=');
	if_pointer_is_null(result);
	(*new_elem)->key = ft_substr(result[0], 0, ft_strlen(result[0]));
	if_value_is_null((*new_elem)->key);
	if (!result[1])
	{
		(*new_elem)->value = ft_substr("\0", 0, 1);
		if_value_is_null((*new_elem)->value);
	}
	else
	{
		(*new_elem)->value = ft_substr(result[1], 0, ft_strlen(result[1]));
		if_value_is_null((*new_elem)->value);
	}
	free_string_array(result);
}

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

void	if_arg_submit_with_equal(char **value, char *str, int *result)
{
	free(*value);
	if (str)
	{
		*value = ft_substr(str, 0, ft_strlen(str));
		if_value_is_null(*value);
	}
	else
	{
		*value = ft_substr("\0", 0, 1);
		if_value_is_null(*value);
	}
	*result = 1;
}

int	find_argument_in_env(char *argument, t_env **env)
{
	char	**sparg;
	int		a_in_e;
	char	*return_strchr;
	t_env	*tmp;
	int		result;

	tmp = *env;
	sparg = ft_split(argument, '=');
	if_pointer_is_null(sparg);
	result = 0;
	while (tmp)
	{
		a_in_e = ft_strcmp(sparg[0], tmp->key);
		return_strchr = ft_strchr(argument, '=');
		if (!a_in_e && return_strchr)
			if_arg_submit_with_equal(&tmp->value, sparg[1], &result);
		else if (!a_in_e && !return_strchr)
			result = 1;
		tmp = tmp->next;
	}
	free_string_array(sparg);
	return (result);
}

int	add_arguments(t_list *cmd, t_env **env)
{
	int		i;
	int		result;

	result = 0;
	i = 1;
	while (cmd->arguments[i])
	{
		if (ft_isalpha(cmd->arguments[i][0]) \
			|| cmd->arguments[i][0] == '_')
		{
			if (!find_argument_in_env(cmd->arguments[i], env))
				env_lstadd_back(env, export_create_elem(cmd->arguments[i]));
		}
		else
		{
			result = 1;
			ft_putstr_fd("minishell: export: '", 2);
			ft_putstr_fd(cmd->arguments[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
		}
		i++;
	}
	return (result);
}
