/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 20:31:29 by fdarkhaw          #+#    #+#             */
/*   Updated: 2022/04/23 22:30:17 by fdarkhaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	token_to_char(int token)
{
	if (token == 206)
		return ('\'');
	if (token == 207)
		return ('\"');
	if (token == 208)
		return ('>');
	if (token == 212)
		return ('(');
	if (token == 213)
		return (')');
	return ('\0');
}

int	check_bad_redirect(t_info *info)
{
	int	i;
	int	redirect;

	i = 0;
	redirect = 0;
	while (i < info->num_of_tokens)
	{
		if (info->tokens[i].type == REDIRECT_APPEND \
								&& info->tokens[i + 1].type == REDIRECT_OUT)
			return (REDIRECT_OUT);
		if (info->tokens[i].type == REDIRECT_APPEND \
								&& info->tokens[i + 1].type == REDIRECT_APPEND)
			return (REDIRECT_APPEND);
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

int	check_bad_syntax(char *str, t_info *info)
{
	int	ret_parenthesis;
	int	ret_qoutes;
	int	ret_redirect;

	(void)str;
	ret_parenthesis = check_bad_parenthesis(info);
	ret_qoutes = check_bad_qoutes(info);
	ret_redirect = check_bad_redirect(info);
	if (ret_parenthesis)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token \'", 2);
		ft_putchar_fd(token_to_char(ret_parenthesis), 2);
		ft_putendl_fd("\'", 2);
		info->status = 258;
		return (1);
	}
	if (ret_qoutes)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token \'", 2);
		ft_putchar_fd(token_to_char(ret_qoutes), 2);
		ft_putendl_fd("\'", 2);
		info->status = 258;
		return (1);
	}
	if (ret_redirect)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token \'", 2);
		ft_putchar_fd(token_to_char(ret_redirect), 2);
		ft_putendl_fd("\'", 2);
		info->status = 258;
		return (1);
	}
	return (0);
}

//1) додумать обработку REDIRECT_APPEND ">>>>"
//2) переписать check_bad_syntax убрать повторы в отдельную функцию
//3) написать обработку limiter'ов, if is_limiter -> то слева и справа не д.б. пусто