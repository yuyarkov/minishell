/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/20 13:00:14 by jg                #+#    #+#             */
/*   Updated: 2022/05/28 16:22:00 by fdarkhaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_exit_command(t_list *cmd)
{
	(void)cmd;
}

int	is_numeric(char *s)
{
	int	i;
	int	result;

	if (!s)
		return (1);
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

int	check_exit(char **split_arg, t_info *info, char *str)
{
	if (!is_numeric(split_arg[1]))
	{
		ft_putstr_fd("exit\nminishell: exit: ", 2);
		ft_putstr_fd(split_arg[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		info->status = 255;
		free_befor_exit(str, split_arg, info);
		return (1);
	}
	else if (split_arg[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		info->status = 1;
		free_string_array(split_arg);
		return (0);
	}
	else if (is_numeric(split_arg[1]) && !split_arg[2])
	{
		ft_putstr_fd("exit\n", 1);
		info->status = ft_atoi(split_arg[1]);
		free_befor_exit(str, split_arg, info);
		return (1);
	}
	free_string_array(split_arg);
	return (0);
}

int	is_exit_command(char *str, t_info *info)
{
	char	**split_arg;

	if (!str)
		return (0);
	split_arg = ft_split(str, ' ');
	if_pointer_is_null(split_arg);
	if ((ft_strcmp(split_arg[0], "exit\0") == 0) && split_arg[1] == NULL)
	{
		ft_putstr_fd("exit\n", 1);
		free_string_array(split_arg);
		exit(info->status);
	}
	if (ft_strcmp(split_arg[0], "exit\0") == 0)
		return (check_exit(split_arg, info, str));
	free_string_array(split_arg);
	return (0);
}
