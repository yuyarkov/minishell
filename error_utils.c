/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/25 16:05:23 by dirony            #+#    #+#             */
/*   Updated: 2022/03/29 15:31:26 by fdarkhaw         ###   ########.fr       */
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

void	print_cmd_error(char *argv)
{
	ft_putstr_fd("minishell: command not found: ", 2);
	ft_putstr_fd(get_cmd_name(argv), 2);
	ft_putstr_fd("\n", 2);
}
