/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jg <jg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/17 17:26:09 by jg                #+#    #+#             */
/*   Updated: 2022/03/17 18:13:58 by jg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



void	env_lstdelone(t_env *list, void (*del)(void *))
{
	del(list->key);
	del(list->value);
	free(list);
}

int	execute_unset_command(t_list *cmd, char **envp, t_env *env)
{
	(void)envp;
	if (cmd->arguments[1])
	{
		while (env)
		{
			if (ft_strncmp(cmd->arguments[1], env->key, ft_strlen(cmd->arguments[1])) == 0)
			{
				env_lstdelone(env, free);// это удаление не работает, сечас минишелл просто закрывается
			}
			env = env->next;
		}
	}
	return (0);
}
