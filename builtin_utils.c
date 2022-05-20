/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 12:42:00 by fdarkhaw          #+#    #+#             */
/*   Updated: 2022/05/20 19:03:57 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	max_strlen(char *str1, char *str2)
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

void	if_pointer_is_null(char **value)
{
	if (NULL == value)
	{
		ft_putstr_fd("Memory has not been properly allocated\n", 2);
		exit(1);
	}
// пока заменил на обычную проверку после маллока
	// int	i;

	// i = 0;
	// while (value && value[i])
	// {
	// 	if (NULL == value[i])
	// 	{
	// 		ft_putstr_fd("Memory has not been properly allocated\n", 2);
	// 		exit(1);
	// 	}
	// 	i++;
	// }
}

void	if_value_is_null(void *value)
{
	if (NULL == value)
	{
		ft_putstr_fd("Memory has not been properly allocated\n", 2);
		exit(1);
	}
}

t_env	*env_create_elem(char *str)
{
	t_env	*new_elem;
	char	**result;

	new_elem = malloc(sizeof(t_env));
	if_value_is_null(new_elem);
	if (ft_strchr(str, '='))
	{
		result = ft_split(str, '=');
		if_pointer_is_null(result);
		new_elem->key = ft_substr(result[0], 0, ft_strlen(result[0]));
		if_value_is_null(new_elem->key);
		if (result[1])
		{
			new_elem->value = ft_substr(result[1], 0, ft_strlen(result[1]));
			if_value_is_null(new_elem->value);
		}
		free_string_array(result);
	}
	else
	{
		new_elem->key = ft_substr(str, 0, ft_strlen(str));
		if_value_is_null(new_elem->key);
		new_elem->value = NULL;
	}
	new_elem->next = NULL;
	return (new_elem);
}
