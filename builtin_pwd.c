/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/20 12:58:06 by jg                #+#    #+#             */
/*   Updated: 2022/05/24 23:07:24 by fdarkhaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_pwd_command(t_list *cmd)
{
	char	buf[1024];

	if (ft_strncmp(cmd->arguments[0], "pwd\0", 4) == 0)
	{
		getcwd(buf, 1024);
		ft_putendl_fd(buf, 1);
	}
	else
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->arguments[0], 2);
		ft_putstr_fd(": command not found\n", 2);
	}
	return (0);
}
