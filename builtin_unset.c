/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/17 17:26:09 by jg                #+#    #+#             */
/*   Updated: 2022/05/24 21:41:51 by fdarkhaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	delete_list_from_env(t_env **list, t_env *tmp, int index)
{
	t_env	*new_env;

	if (index == 0)
	{
		new_env = (*list)->next;
		**list = *new_env;
	}
	else
	{
		new_env = *list;
		while (index - 1)
		{
			new_env = new_env->next;
			index--;
		}
		new_env->next = tmp;
		*list = new_env;
	}
}

void	search_list(t_env **list, char *str)
{
	t_env	*tmp;
	int		index;

	tmp = *list;
	index = 0;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, str) == 0)
		{
			free(tmp->key);
			free(tmp->value);
			free(tmp);
			delete_list_from_env(list, tmp->next, index);
			break ;
		}
		tmp = tmp->next;
		index++;
	}
}

int	execute_unset_command(t_list *cmd, t_env *env)
{
	int		i;

	i = 1;
	while (cmd->arguments[i])
	{
		search_list(&env, cmd->arguments[i]);
		i++;
	}
	return (0);
}
