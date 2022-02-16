/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/18 19:45:15 by dirony            #+#    #+#             */
/*   Updated: 2022/02/16 21:53:40 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*create_elem(char *argv, char **envp)
{
	t_list	*new_elem;

	new_elem = malloc(sizeof(t_list));
	if (NULL == new_elem)
		return (NULL);
	new_elem->next = NULL;
	new_elem->previous = NULL;
	new_elem->cmd = get_cmd_path(argv, envp);
	if (!new_elem->cmd)
		print_cmd_error(argv);
	new_elem->end[0] = 0;
	new_elem->end[1] = 0;
	return (new_elem);
}

t_list	*add_cmd_to_list(int argc, char **argv, char **envp)
{
	int		i;
	t_list	*new_elem;
	t_list	*list;
	char	*temp;

	i = 0;
	new_elem = NULL;
	list = create_elem(argv[i], envp);
	while (++i < argc - 1)
	{
		temp = get_cmd_path(argv[i], envp);
		printf("cmd_path: %s\n", temp);
		if (temp)
		{	
			new_elem = create_elem(argv[i], envp);
			ft_lstadd_back(&list, new_elem);
		}
		else
			print_cmd_error(argv[i]);
		free(temp);
	}
	return (list);
}
