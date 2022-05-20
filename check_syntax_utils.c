/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 20:31:29 by fdarkhaw          #+#    #+#             */
/*   Updated: 2022/05/20 20:36:01 by fdarkhaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_bad_limiter(t_token *t, int not)
{
	int	i;

	i = 0;
	while (i < not)
	{
		if (t[i].type == AND_SIGN && ((t[i - 1].type != WORD \
		&& t[i - 1].type != RIGHT_PARENTHESIS && t[i - 1].type != DOUBLE_QUOTE) \
		|| (t[i + 1].type != WORD && t[i + 1].type != LEFT_PARENTHESIS)))
			return (AND_SIGN);
		else if (t[i].type == OR_SIGN && ((t[i - 1].type != WORD \
		&& t[i - 1].type != RIGHT_PARENTHESIS && t[i - 1].type != DOUBLE_QUOTE) \
		|| (t[i + 1].type != WORD && t[i + 1].type != LEFT_PARENTHESIS)))
			return (OR_SIGN);
		else if (t[i].type == PIPE && ((t[i - 1].type != WORD \
		&& t[i - 1].type != RIGHT_PARENTHESIS && t[i - 1].type != DOUBLE_QUOTE) \
		|| (t[i + 1].type != WORD && t[i + 1].type != LEFT_PARENTHESIS)))
			return (PIPE);
		i++;
	}
	return (0);
}

int	check_bad_redirect(t_token *t, int not)
{
	int	i;

	i = 0;
	while (i < not)
	{
		if (t[i].type == REDIRECT_OUT && t[i + 1].type != WORD)
			return (REDIRECT_OUT);
		else if (t[i].type == REDIRECT_IN && t[i + 1].type != WORD)
			return (REDIRECT_IN);
		else if (t[i].type == REDIRECT_APPEND && t[i + 1].type != WORD)
			return (NEW_LINE);
		else if (t[i].type == REDIRECT_HEREDOC && t[i + 1].type != WORD)
			return (NEW_LINE);
		else if (t[i].type == REDIRECT_APPEND && t[i + 1].type == REDIRECT_OUT)
			return (REDIRECT_OUT);
		else if (t[i].type == REDIRECT_APPEND && t[i + 1].type == 210)
			return (REDIRECT_APPEND);
		else if (t[i].type == REDIRECT_OUT && t[i + 1].type == REDIRECT_IN)
			return (REDIRECT_IN);
		else if (t[i].type == REDIRECT_APPEND && t[i + 1].type == REDIRECT_IN)
			return (REDIRECT_IN);
		i++;
	}
	return (0);
}

int	check_bad_parenthesis(t_token *t, int not)
{
	int	i;
	int	parenthesis;

	i = 0;
	parenthesis = 0;
	while (i < not)
	{
		if (t[i].type == LEFT_PARENTHESIS)
			parenthesis++;
		else if (t[i].type == RIGHT_PARENTHESIS)
			parenthesis--;
		if (parenthesis < 0)
			return (RIGHT_PARENTHESIS);
		i++;
	}
	if (parenthesis != 0)
		return (LEFT_PARENTHESIS);
	return (0);
}

int	check_bad_qoutes(t_token *t, int not)
{
	int	i;
	int	quote;
	int	double_quote;

	i = 0;
	quote = 0;
	double_quote = 0;
	while (i < not)
	{
		if (t[i].type == QUOTE)
			quote = quote ^ 1;
		else if (t[i].type == DOUBLE_QUOTE)
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

	ret_parenthesis = check_bad_parenthesis(info->tokens, info->num_of_tokens);
	ret_qoutes = check_bad_qoutes(info->tokens, info->num_of_tokens);
	ret_redirect = check_bad_redirect(info->tokens, info->num_of_tokens);
	ret_limiter = check_bad_limiter(info->tokens, info->num_of_tokens);
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
