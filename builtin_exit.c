/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/20 13:00:14 by jg                #+#    #+#             */
/*   Updated: 2022/05/27 21:12:49 by fdarkhaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_exit_command(t_list *cmd)
{
	(void)cmd;
		// ft_putstr_fd("minishell: exit: too many arguments\n", 2); //Юра - проверить fd для вывода ошибки
	printf("inside execute_exit \n");
	return (1); //в этом случае из минишелла не выходим
}
