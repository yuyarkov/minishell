/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/25 16:05:23 by dirony            #+#    #+#             */
/*   Updated: 2022/05/03 14:44:20 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_cmd_error(char *argv)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(get_first_word(argv), 2);
	ft_putendl_fd(": command not found - old message", 2);
}

void	print_file_error(char *file_name)
{
	ft_putstr_fd(SHELL, 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(file_name, 2);
	ft_putstr_fd("\n", 2);
	exit (EXIT_FAILURE);
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
