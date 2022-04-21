/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 20:31:29 by fdarkhaw          #+#    #+#             */
/*   Updated: 2022/04/21 20:52:40 by fdarkhaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_bad_qoute(t_info *info)
{
	int	i;
	int	quote;

	i = 0;
	quote = 0;
	while (i < info->num_of_tokens)
	{
		if (info->tokens[i].type == QUOTE)
			quote = quote ^ 1;
		i++;
	}
	// printf("quote = %d\n", quote);
	if (quote)
		return (1);
	return (0);
}

int	check_bad_syntax(char *str, t_info *info)
{
	(void)str;
	if (check_bad_qoute(info))//прописать условия входа
	{
		ft_putstr_fd("minishell: syntax error near unexpected token \'", 2);
		ft_putstr_fd("ANY BAD TOKEN", 2);
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