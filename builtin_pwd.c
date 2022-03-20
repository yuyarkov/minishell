/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jg <jg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/20 12:58:06 by jg                #+#    #+#             */
/*   Updated: 2022/03/20 13:01:41 by jg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_pwd_command(t_list *cmd, char **envp)
{
	char	buf[1024];

	(void) envp;
	//printf("inside execute_pwd\n");//
	if (ft_strncmp(cmd->arguments[0], "pwd\0", 4) == 0)
	{
		getcwd(buf, 1024);
		ft_putendl_fd(buf, 1);
	}
	else
	{
		ft_putstr_fd(cmd->arguments[0], 2);
		ft_putstr_fd(": command not found\n", 2);
	}
	return (0);
}
