/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/25 16:05:23 by dirony            #+#    #+#             */
/*   Updated: 2022/03/30 21:02:19 by fdarkhaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_file_error(char *file_name)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(file_name, 2);
	exit (EXIT_FAILURE);
}

int	print_cmd_error(char *argv)// должна изменять статус выхода
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(get_cmd_name(argv), 2);
	ft_putendl_fd(": No such file or directory", 2);
	return (127);
}
