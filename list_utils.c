/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/18 19:45:15 by dirony            #+#    #+#             */
/*   Updated: 2022/03/14 16:48:14 by fdarkhaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*create_elem(char *str, char **envp)
{
	t_list	*new_elem;

	new_elem = malloc(sizeof(t_list));
	if (NULL == new_elem)
		return (NULL);
	new_elem->next = NULL;
	new_elem->previous = NULL;
	if (is_builtin_command(str))
		new_elem->cmd = str;
	else
		new_elem->cmd = get_cmd_path(str, envp);
	if (!new_elem->cmd)
		print_cmd_error(str);
	new_elem->arguments = ft_split(str, ' ');
	new_elem->end[0] = 0;//для пайпов пригодится
	new_elem->end[1] = 0;
	return (new_elem);
}

t_list	*add_cmd_to_list(int argc, char **commands, char **envp)
{
	int		i;
	t_list	*new_elem;
	t_list	*list;
	//char	*temp;

	new_elem = NULL;
	list = create_elem(commands[0], envp);
	i = 1;
	while (i < argc)
	{
		// temp = get_cmd_path(commands[i], envp);
		// if (temp)
		// {	
			new_elem = create_elem(commands[i], envp);
			ft_lstadd_back(&list, new_elem);
		// }
		// else
		// 	print_cmd_error(commands[i]);
		// free(temp);
		i++;
	}
	return (list);
}
