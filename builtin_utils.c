/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 12:42:00 by fdarkhaw          #+#    #+#             */
/*   Updated: 2022/05/28 16:55:05 by fdarkhaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	if_pointer_is_null(char **value)
{
	if (NULL == value)
	{
		ft_putstr_fd("Memory has not been properly allocated\n", 2);
		exit(1);
	}
}

void	if_value_is_null(void *value)
{
	if (NULL == value)
	{
		ft_putstr_fd("Memory has not been properly allocated\n", 2);
		exit(1);
	}
}

void	if_equal_in_str(char *str, t_env **new_elem)
{
	char	**result;

	result = ft_split(str, '=');
	if_pointer_is_null(result);
	(*new_elem)->key = ft_substr(result[0], 0, ft_strlen(result[0]));
	if_value_is_null((*new_elem)->key);
	if (result[1])
	{
		(*new_elem)->value = ft_substr(result[1], 0, ft_strlen(result[1]));
		if_value_is_null((*new_elem)->value);
	}
	free_string_array(result);
}

t_env	*env_create_elem(char *str)
{
	t_env	*new_elem;

	new_elem = malloc(sizeof(t_env));
	if_value_is_null(new_elem);
	if (ft_strchr(str, '='))
		if_equal_in_str(str, &new_elem);
	else
	{
		new_elem->key = ft_substr(str, 0, ft_strlen(str));
		if_value_is_null(new_elem->key);
		new_elem->value = NULL;
	}
	new_elem->next = NULL;
	return (new_elem);
}
