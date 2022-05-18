/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 12:42:00 by fdarkhaw          #+#    #+#             */
/*   Updated: 2022/05/18 21:40:01 by fdarkhaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_max_strlen(char *str1, char *str2)
{
	int	s1;
	int	s2;

	s1 = ft_strlen(str1);
	s2 = ft_strlen(str2);
	if (s1 > s2)
		return (s1);
	else
		return (s2);
}

t_env	*env_create_elem(char *str)
{
	t_env	*new_elem;
	char	**result;

	new_elem = malloc(sizeof(t_env));
	if (!new_elem)
		exit(1);
	new_elem->next = NULL;
	if (ft_strchr(str, '='))
	{	
		result = ft_split(str, '=');
		if (!result)
			exit(1);
		new_elem->key = ft_substr(result[0], 0, ft_strlen(result[0]));
		if (!new_elem->key)
			exit(1);
		if (result[1])
		{
			new_elem->value = ft_substr(result[1], 0, ft_strlen(result[1]));
			if (!new_elem->value)
				exit(1);
		}
		free_string_array(result);
	}
	else
	{
		new_elem->key = ft_substr(str, 0, ft_strlen(str));
		if (!new_elem->key)
			exit(1);
		new_elem->value = NULL;
	}
	return (new_elem);
}
