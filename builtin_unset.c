/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/17 17:26:09 by jg                #+#    #+#             */
/*   Updated: 2022/03/18 19:27:24 by fdarkhaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	del_list_from_env(t_env **list, t_env *tmp, int iterator)
{
	t_env	*new_env;

	new_env = *list;
	while (iterator - 1)//нахожу лист перед удаляемым
	{
		new_env = new_env->next;
		iterator--;
	}
	new_env->next = tmp;//связываю эл перед удаляем с эл после удаляемого
	*list = new_env;
}

void	search_and_del(t_env **list, char *str)
{
	t_env	*tmp;
	int		iterator;

	tmp = *list;
	iterator = 0;
	while (tmp)// пока список есть
	{
		if (ft_strncmp(tmp->key, str, ft_strlen(tmp->key)) == 0)//если ключ совпал
		{
			printf("HERE\n");//ищу ошибку дальше отсюда
			del_list_from_env(list, tmp->next, iterator);//далле работаю с енв, элементом след за удаляемым и номером удаляемого элта
		}
		tmp = tmp->next;
		iterator++;
	}
}

int	execute_unset_command(t_list *cmd, char **envp, t_env *env)
{
	(void)envp;
	if (cmd->arguments[1])
		search_and_del(&env, cmd->arguments[1]);
	return (0);
}
