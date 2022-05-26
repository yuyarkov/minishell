/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/20 13:00:14 by jg                #+#    #+#             */
/*   Updated: 2022/05/26 21:20:08 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_numeric(char *s)
{
	int	i;
	int	result;

	if (!s)
		return (0);
	result = 1;
	i = 0;
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			result = 0;
		i++;
	}
	return (result);
}

int	execute_exit_command(t_list *cmd)
{
	int		exit_code;

	ft_putstr_fd("exit\n", 1);
	if (!cmd->arguments[1])
		return (0);
	if (!is_numeric(cmd->arguments[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(cmd->arguments[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		exit(255);
	}
	else if (cmd->arguments[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	else if (is_numeric(cmd->arguments[1]) && !cmd->arguments[2])
	{
		exit_code = ft_atoi(cmd->arguments[1]);
		exit(exit_code);
	}
	return (0);
}
