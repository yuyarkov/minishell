/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 15:53:52 by fdarkhaw          #+#    #+#             */
/*   Updated: 2022/05/24 21:34:54 by fdarkhaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_export_command(t_list *cmd, t_env *env)
{
	int		result;

	result = 0;
	if (cmd->arguments[1])
		result = add_arguments(cmd, &env);
	else
		create_sort_env(env);
	return (result);
}
