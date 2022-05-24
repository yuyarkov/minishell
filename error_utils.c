/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/25 16:05:23 by dirony            #+#    #+#             */
/*   Updated: 2022/05/24 20:11:22 by fdarkhaw         ###   ########.fr       */
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
	info->status = 25;//258;
	return (1);
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
