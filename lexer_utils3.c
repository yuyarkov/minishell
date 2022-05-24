/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/20 21:25:35 by dirony            #+#    #+#             */
/*   Updated: 2022/05/22 20:34:53 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	put_word_token_in_quotes(char *s, t_token *t, int *k)
{
	int		i;
	char	*result;

	result = malloc(ft_strlen(s) + 1);
	if (NULL == result)
		exit(EXIT_FAILURE);
	i = 0;
	while (s[i] && s[i] != '\"' && s[i] != '$')
	{
		result[i] = s[i];
		i++;
	}
	result[i] = '\0';
	t->value = result;
	t->type = WORD;
	t->in_qoutes = 1;
	*k = *k + 1;
	return (i);
}

int	put_token_symbol_double_qoute(t_token *t, int *k)
{
	t->type = DOUBLE_QUOTE;
	*k = *k + 1;
	return (1);
}

int	put_token_from_double_quotes(char *s, t_token *t, int *k)
{
	int		i;

	i = put_token_symbol_double_qoute(t, k);
	t = &t[1];
	while (s[i] && s[i] != '\"')
	{
		if (s[i] && s[i] != '$')
		{
			i = i + put_word_token_in_quotes(&s[i], t, k);
			t = &t[1];
		}
		if (s[i] && s[i] == '$')
		{
			t->in_qoutes = 1;
			i = i + put_dollar_key_to_token(&s[i], t, k);
			t = &t[1];
		}
		if (s[i] && s[i] == '\"')
		{
			i = i + put_token_symbol_double_qoute(t, k);
			t = &t[1];
		}
	}
	return (i);
}
