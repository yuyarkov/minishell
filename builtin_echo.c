/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/20 13:02:20 by jg                #+#    #+#             */
/*   Updated: 2022/05/24 21:37:13 by fdarkhaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_echo_command(t_list *cmd)
{
	int		iterator;

	iterator = 1;
	if (ft_strncmp(cmd->arguments[1], "-n\0", 3) == 0)
		iterator = 2;
	while (cmd->arguments[iterator])
	{
		ft_putstr_fd(cmd->arguments[iterator++], 1);
		if (cmd->arguments[iterator])
			ft_putchar_fd(' ', 1);
	}
	if (ft_strncmp(cmd->arguments[1], "-n\0", 3) != 0)
		ft_putchar_fd('\n', 1);
	return (0);
}
