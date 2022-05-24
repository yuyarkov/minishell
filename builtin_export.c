/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 15:53:52 by fdarkhaw          #+#    #+#             */
/*   Updated: 2022/05/24 20:51:50 by fdarkhaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_export_command(t_list *cmd, char **envp, t_env *env)
{
	int		result;
	t_env	*tmp;

	(void)envp;
	result = 0;
	if (cmd->arguments[1])
		result = add_arguments(cmd, &env);
	else
	{
		tmp = create_sort_env(env);
		print_export(tmp);
		lstiter_env(tmp, free);
	}
	return (result);
}
