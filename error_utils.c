/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/25 16:05:23 by dirony            #+#    #+#             */
/*   Updated: 2022/04/26 19:53:09 by fdarkhaw         ###   ########.fr       */
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
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(get_first_word(argv), 2);
	ft_putendl_fd(": command not found - old message", 2);
}

// void	print_cmd_error(char *argv)
// {
// 	int		i;
// 	char	*str;

// 	i = 0;
// 	str = get_first_word(argv);
// 	printf("str = %s\n", str);
// 	ft_putstr_fd("minishell: ", 2);
// 	while (str[i])
// 	{
// 		if (!ft_memchr("\'\"", str[i], 2))
// 			ft_putchar_fd(str[i], 2);
// 		i++;
// 	}
// 	ft_putendl_fd(": command not found", 2);
// }
