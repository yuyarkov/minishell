/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jg <jg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/20 13:00:14 by jg                #+#    #+#             */
/*   Updated: 2022/03/20 13:03:07 by jg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_numeric(char *s)
{
	int	i;
	int	result;

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

int	execute_exit_command(t_list *cmd, char **envp)
{
	int		exit_code;

	(void) envp;
	ft_putstr_fd("exit\n", 1);
	if (!is_numeric(cmd->arguments[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(cmd->arguments[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		exit(255);//bash закрывается с кодом 255
	}
	else if (cmd->arguments[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2); //Юра - проверить fd для вывода ошибки
		return (1); //в этом случае из минишелла не выходим
	}
	else if (is_numeric(cmd->arguments[1]) && !cmd->arguments[2])
	{
		exit_code = ft_atoi(cmd->arguments[1]);
		exit(exit_code);
	}
	return (0);// до этого момента не доходит, т.к. при exit без аргументов останавливается цикле в main
//Юра - какой должен быть код?
//Артём - если значение exit_code не указано, статусом выхода будет статус выхода последней выполненной команды
//сейчас так и работает
}
