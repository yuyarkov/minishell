/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/27 21:44:41 by dirony            #+#    #+#             */
/*   Updated: 2022/05/28 15:50:59 by fdarkhaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_values_to_null(t_list *iter)
{
	iter->cmd = NULL;
	iter->arguments = NULL;
	iter->redirect_in_file = NULL;
	iter->out_file = NULL;
}

void	clear_cmd(t_list *iter)
{
	t_list	*temp;

	while (iter)
	{
		if (iter->cmd)
			free(iter->cmd);
		if (iter->arguments)
			free_string_array(iter->arguments);
		if (iter->redirect_in_file)
			free(iter->redirect_in_file);
		if (iter->out_file)
			free(iter->out_file);
		set_values_to_null(iter);
		temp = iter;
		iter = iter->next;
		free(temp);
	}
}

void	clear_info_except_envp(t_info *info)
{
	clear_tokens(info);
	if (info->envp)
		free_string_array(info->envp);
	info->commands = NULL;
}
