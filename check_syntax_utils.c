/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 20:31:29 by fdarkhaw          #+#    #+#             */
/*   Updated: 2022/05/15 14:57:39 by fdarkhaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	write_token(int token)
{
	if (token == 204)
		ft_putchar_fd('|', 2);
	else if (token == 206)
		ft_putchar_fd('\'', 2);
	else if (token == 207)
		ft_putchar_fd('\"', 2);
	else if (token == 208)
		ft_putchar_fd('>', 2);
	else if (token == 209)
		ft_putchar_fd('<', 2);
	else if (token == 212)
		ft_putchar_fd('(', 2);
	else if (token == 213)
		ft_putchar_fd(')', 2);
	else if (token == 201)
		ft_putstr_fd("&&", 2);
	else if (token == 203)
		ft_putstr_fd("||", 2);
	else if (token == 210)
		ft_putstr_fd(">>", 2);
	else if (token == 211)
		ft_putstr_fd("<<", 2);
}

int	print_error_token(t_info *info, int token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token \'", 2);
	write_token(token);
	ft_putendl_fd("\'", 2);
	info->status = 25;//258;
	return (1);
}

int	check_bad_limiter(t_info *info)
{
	int	i;

	i = 0;
	while (i < info->num_of_tokens)
	{
		if (info->tokens[i].type == AND_SIGN \
							&& ((info->tokens[i - 1].type != WORD \
							&& info->tokens[i - 1].type != RIGHT_PARENTHESIS) \
							|| (info->tokens[i + 1].type != WORD \
							&& info->tokens[i + 1].type != LEFT_PARENTHESIS)))
			return (AND_SIGN);
		else if (info->tokens[i].type == OR_SIGN \
							&& ((info->tokens[i - 1].type != WORD \
							&& info->tokens[i - 1].type != RIGHT_PARENTHESIS) \
							|| (info->tokens[i + 1].type != WORD \
							&& info->tokens[i + 1].type != LEFT_PARENTHESIS)))
			return (OR_SIGN);
		else if (info->tokens[i].type == PIPE \
							&& ((info->tokens[i - 1].type != WORD \
							&& info->tokens[i - 1].type != RIGHT_PARENTHESIS) \
							|| (info->tokens[i + 1].type != WORD \
							&& info->tokens[i + 1].type != LEFT_PARENTHESIS)))
			return (PIPE);
		i++;
	}
	return (0);
}

int	check_bad_redirect(t_info *info)
{
	int	i;

	i = 0;
	while (i < info->num_of_tokens)
	{
		// printf("%d\n", info->tokens[i].type);
		if (info->tokens[i].type == REDIRECT_OUT \
								&& info->tokens[i + 1].type != WORD)
			return (REDIRECT_OUT);
		else if (info->tokens[i].type == REDIRECT_IN \
								&& info->tokens[i + 1].type != WORD)
			return (REDIRECT_IN);
		else if (info->tokens[i].type == REDIRECT_APPEND \
								&& info->tokens[i + 1].type != WORD)
			return (REDIRECT_APPEND);//должен быть newline
		else if (info->tokens[i].type == REDIRECT_HEREDOC \
								&& info->tokens[i + 1].type != WORD)
			return (REDIRECT_HEREDOC);//должен быть newline
		else if (info->tokens[i].type == REDIRECT_APPEND \
								&& info->tokens[i + 1].type == REDIRECT_OUT)
			return (REDIRECT_OUT);
		else if (info->tokens[i].type == REDIRECT_APPEND \
								&& info->tokens[i + 1].type == REDIRECT_APPEND)
			return (REDIRECT_APPEND);
		else if (info->tokens[i].type == REDIRECT_OUT \
								&& info->tokens[i + 1].type == REDIRECT_IN)
			return (REDIRECT_IN);
		else if (info->tokens[i].type == REDIRECT_APPEND \
								&& info->tokens[i + 1].type == REDIRECT_IN)
			return (REDIRECT_IN);
		i++;
	}
	return (0);
}

int	check_bad_parenthesis(t_info *info)
{
	int	i;
	int	parenthesis;

	i = 0;
	parenthesis = 0;
	while (i < info->num_of_tokens)
	{
		if (info->tokens[i].type == LEFT_PARENTHESIS)
			parenthesis++;
		else if (info->tokens[i].type == RIGHT_PARENTHESIS)
			parenthesis--;
		if (parenthesis < 0)
			return (RIGHT_PARENTHESIS);
		i++;
	}
	if (parenthesis != 0)
		return (LEFT_PARENTHESIS);
	return (0);
}

int	check_bad_qoutes(t_info *info)
{
	int	i;
	int	quote;
	int	double_quote;

	i = 0;
	quote = 0;
	double_quote = 0;
	while (i < info->num_of_tokens)
	{
		if (info->tokens[i].type == QUOTE)
			quote = quote ^ 1;
		else if (info->tokens[i].type == DOUBLE_QUOTE)
			double_quote = double_quote ^ 1;
		i++;
	}
	if (quote)
		return (QUOTE);
	else if (double_quote)
		return (DOUBLE_QUOTE);
	return (0);
}

int	check_bad_syntax(t_info *info)
{
	int	ret_parenthesis;
	int	ret_qoutes;
	int	ret_redirect;
	int	ret_limiter;

	ret_parenthesis = check_bad_parenthesis(info);
	ret_qoutes = check_bad_qoutes(info);
	ret_redirect = check_bad_redirect(info);
	ret_limiter = check_bad_limiter(info);
	if (ret_parenthesis)
		return (print_error_token(info, ret_parenthesis));
	if (ret_qoutes)
		return (print_error_token(info, ret_qoutes));
	if (ret_redirect)
		return (print_error_token(info, ret_redirect));
	if (ret_limiter)
		return (print_error_token(info, ret_limiter));
	return (0);
}
