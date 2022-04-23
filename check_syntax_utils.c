/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 20:31:29 by fdarkhaw          #+#    #+#             */
/*   Updated: 2022/04/23 17:58:30 by fdarkhaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	token_to_char(int token)
{
	if (token == 206)
		return ('\'');
	if (token == 207)
		return ('\"');
	if (token == 212)
		return ('(');
	if (token == 213)
		return (')');
	return ('\0');
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
			return (i);
		i++;
	}
	if (parenthesis != 0)
	{
		printf("here\n");
		return (i - 2);
	}
	// printf("i = %d\n", i);
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
		return (1);
	else if (double_quote)
		return (1);
	return (0);
}

int	check_bad_syntax(char *str, t_info *info)
{
	int	ret;

	(void)str;
	ret = check_bad_parenthesis(info);
	printf("ret = %d\n", ret);
	if (ret)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token \'", 2);
		ft_putchar_fd(token_to_char(info->tokens[ret].type), 2);
		ft_putendl_fd("\'", 2);
		info->status = 258;
		return (1);
	}
	else if (check_bad_qoutes(info))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token \'", 2);
		ft_putstr_fd("BAD QOUTES", 2);
		ft_putendl_fd("\'", 2);
		info->status = 258;
		return (1);
	}

	return (0);
}

// void	print_tokens(t_info *info)
// {
// 	int	i;

// 	printf("Num of tokens: %d\n", info->num_of_tokens);
// 	i = 0;
// 	while (i < info->num_of_tokens)
// 	{
// 		printf("token %d\ttype: %d,\tvalue: %s\n", i, info->tokens[i].type, info->tokens[i].value);
// 		i++;
// 	}
// }