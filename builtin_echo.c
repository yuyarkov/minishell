/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/20 13:02:20 by jg                #+#    #+#             */
/*   Updated: 2022/03/31 17:04:51 by fdarkhaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_echo_command(t_list *cmd, char **envp)
{
	int		iterator;

	(void) envp;
	iterator = 1;//Артём - при 1 флаг -n печатается))
				//Юра - почему 2? заменил на 1, заработало
	if (ft_strncmp(cmd->arguments[1], "-n\0", 3) == 0)//если есть -n iterator = 2
		iterator = 2;
	while (cmd->arguments[iterator])
	{
		ft_putstr_fd(cmd->arguments[iterator++], 1);
		if (cmd->arguments[iterator])
			ft_putchar_fd(' ', 1);
	}
	if (ft_strncmp(cmd->arguments[1], "-n\0", 3) != 0)//если нет -n пиши перевод каретки
		ft_putchar_fd('\n', 1);
	return (0);
}
