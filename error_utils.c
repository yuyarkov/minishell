/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/25 16:05:23 by dirony            #+#    #+#             */
/*   Updated: 2022/05/29 15:36:10 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_cmd_error(char *argv, int *status)
{
	if (argv && *argv != '\0')
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(argv, 2);
		ft_putendl_fd(": command not found", 2);
		*status = 127;
	}
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

void	write_token(int token)
{
	if (token == PIPE)
		ft_putchar_fd('|', 2);
	else if (token == QUOTE)
		ft_putchar_fd('\'', 2);
	else if (token == DOUBLE_QUOTE)
		ft_putchar_fd('\"', 2);
	else if (token == REDIRECT_OUT)
		ft_putchar_fd('>', 2);
	else if (token == REDIRECT_IN)
		ft_putchar_fd('<', 2);
	else if (token == LEFT_PARENTHESIS)
		ft_putchar_fd('(', 2);
	else if (token == RIGHT_PARENTHESIS)
		ft_putchar_fd(')', 2);
	else if (token == AND_SIGN)
		ft_putstr_fd("&&", 2);
	else if (token == OR_SIGN)
		ft_putstr_fd("||", 2);
	else if (token == REDIRECT_APPEND)
		ft_putstr_fd(">>", 2);
	else if (token == REDIRECT_HEREDOC)
		ft_putstr_fd("<<", 2);
	else if (token == NEW_LINE)
		ft_putstr_fd("newline", 2);
}

int	print_error_token(t_info *info, int token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token \'", 2);
	write_token(token);
	ft_putendl_fd("\'", 2);
	info->status = 258;
	return (1);
}
