/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jg <jg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/17 17:26:09 by jg                #+#    #+#             */
/*   Updated: 2022/03/20 20:44:14 by jg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	delete_list_from_env(t_env **list, t_env *tmp, int index)
{
	t_env	*new_env;

	if (index == 0)// если нужно удалить первый элемент
	{
		new_env = (*list)->next;
		**list = *new_env;//обращаюсь к голове связного списка
		// *list = new_env;//изменяю содержимое env
	}
	else
	{
		new_env = *list;
		while (index - 1)//нахожу лист перед удаляемым
		{
			new_env = new_env->next;
			index--;
		}
		new_env->next = tmp;//связываю эл перед удаляем с эл после удаляемого
		*list = new_env;//изменяю содержимое env
		// free(new_env->next->key);
		// free(new_env->next->value);
	}
}

void	search_list(t_env **list, char *str)
{
	t_env	*tmp;
	int		index;

	tmp = *list;
	index = 0;
	while (tmp)// пока список есть
	{
		if (ft_strncmp(tmp->key, str, ft_strlen(tmp->key)) == 0)//если ключ совпал с переданным значением
		{
			free(tmp->key);//освобождаю удаляемый элемент
			free(tmp->value);//нужно чистить сам элемент списка?
			delete_list_from_env(list, tmp->next, index);//далее работаю с: енв, элементом след за удаляемым и номером удаляемого элта
			break ;
		}
		tmp = tmp->next;
		index++;
	}
}

int	execute_unset_command(t_list *cmd, char **envp, t_env *env)
{
	(void)envp;
	if (cmd->arguments[1])
		search_list(&env, cmd->arguments[1]);
	return (0);
}
