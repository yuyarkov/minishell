/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/20 19:33:17 by dirony            #+#    #+#             */
/*   Updated: 2022/05/20 21:33:16 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	put_dollar_key_to_token(char *s, t_token *t, int *k)
{
	char	*key;
	int		i;

	key = malloc(ft_strlen(s) + 1);
	if (NULL == key)
		exit(EXIT_FAILURE);
	i = 1;
	if (s[1] && s[1] == '?')
	{
		key = "?";
		t->type = DOLLAR_KEY;
		t->value = key;
		*k = *k + 1;
		return (i + 1);
	}
	while (s[i] && (ft_isalnum(s[i]) || s[i] == '_' ))
	{
		key[i - 1] = s[i];
		i++;
	}
	key[i - 1] = '\0';
	t->type = DOLLAR_KEY;
	t->value = key;
	*k = *k + 1;
	return (i);
}

int	put_special_token(char *s, t_token *token, int *k)
{
	token->value = NULL;
	if (has_double_special_symbol(s))
		return (put_double_special_token(s, token, k));
	if (*s == '|')
		token->type = PIPE;
	if (*s == '\'')
		return (put_token_from_single_quotes(s, token, k));
	if (*s == '\"')
		return (put_token_from_double_quotes(s, token, k));
	if (*s == '(')
		token->type = LEFT_PARENTHESIS;
	if (*s == ')')
		token->type = RIGHT_PARENTHESIS;
	if (*s == '\\')
		token->type = BACKSLASH;
	if (*s == '$')
		return (put_dollar_key_to_token(s, token, k));
	if (*s == '>')
		token->type = REDIRECT_OUT;
	if (*s == '<')
		token->type = REDIRECT_IN;
	*k = *k + 1;
	return (1);
}

t_token	*init_tokens(char *s)
{
	int		i;
	t_token	*result;

	result = malloc(sizeof(t_token) * (ft_strlen(s) + 1));
	if (NULL == result)
		exit(EXIT_FAILURE);
	i = 0;
	while (s[i])
	{
		result[i] = (t_token){};
		result[i].status = NEVER_EXECUTED;
		i++;
	}
	return (result);
}

void	get_tokens_from_string(char *s, t_info *info)
{
	int		i;
	int		k;
	t_token	*result;

	i = 0;
	k = 0;
	result = init_tokens(s);
	while (s[i])
	{
		while (s[i] && ft_strchr(SPACES, s[i]))
			i++;
		if (s[i])
		{
			if (!ft_strchr(SPECIAL_SYMBOLS, s[i]))
				i = i + put_word_token(&s[i], &result[k], &k);
			else
				i = i + put_special_token(&s[i], &result[k], &k);
		}
	}
	result[k].type = END_OF_TOKENS;
	info->tokens = result;
	info->num_of_tokens = k;
	put_tree_level_marks(info);
	put_group_id_marks(info);
	put_tree_marks(info);
}