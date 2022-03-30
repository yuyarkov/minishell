/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/18 19:45:15 by dirony            #+#    #+#             */
/*   Updated: 2022/03/30 21:11:34 by fdarkhaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*create_elem(char *str, char **envp, int *status)
{
	t_list	*new_elem;

	//printf("inside create elem\n");
	new_elem = malloc(sizeof(t_list));
	if (NULL == new_elem)
		return (NULL);
	new_elem->next = NULL;
	new_elem->previous = NULL;
	if (is_builtin_command(str) || *str == '\0')
		new_elem->cmd = str;
	else
		new_elem->cmd = get_cmd_path(str, envp);
	//printf("new_elem->cmd: %s\n", new_elem->cmd);
	if (!new_elem->cmd)
	{
		*status = print_cmd_error(str);// должна изменять статус выхода на 127
		return (NULL);//здесь выполнение команды должно прерваться и возвращаемся в строку приглашения
	}
	new_elem->arguments = ft_split(str, ' ');
	new_elem->end[0] = 0;//для пайпов пригодится
	new_elem->end[1] = 0;
	return (new_elem);
}

void	ft_double_list_add_back(t_list **list, t_list *new_elem)
{
	t_list	*iterator;

	iterator = *list;
	if (*list)
	{
		while (iterator->next)
		{
		//	printf("inside add_back, iterator->cmd: %s\n", iterator->cmd);
			iterator = iterator->next;
		}
		iterator->next = new_elem;
		new_elem->previous = iterator;
	}
	else
		*list = new_elem;
}

t_list	*add_cmd_to_list(t_info *info, char **commands, char **envp, int *stat)
{
	int		i;
	t_list	*new_elem;
	t_list	*list;
	//char	*temp;

	new_elem = NULL;
	list = create_elem(commands[0], envp, stat);
	if (!list)
		return (NULL);
	list->limiter = info->limiters[0].sign;
	//printf("inside add_cmd\n");
	i = 1;
	while (i < info->num_of_commands)
	{
		// temp = get_cmd_path(commands[i], envp);
		// if (temp)
		// {	
			new_elem = create_elem(commands[i], envp, stat);
			if (!new_elem)
				return (NULL);//добавить очищение списка, т.к. будут утечки
			new_elem->limiter = info->limiters[i].sign;
			ft_double_list_add_back(&list, new_elem);
		// }
		// else
		// 	print_cmd_error(commands[i]);
		// free(temp);
		i++;
	}
	return (list);
}
