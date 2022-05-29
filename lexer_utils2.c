/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/20 19:33:17 by dirony            #+#    #+#             */
/*   Updated: 2022/05/29 13:12:50 by dirony           ###   ########.fr       */
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
	*k = *k + 1;
	if (s[1] && s[1] == '?')
	{
		key[0] = '?';
		key[1] = '\0';
		t->type = DOLLAR_KEY;
		t->value = key;
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
	size_t	i;
	t_token	*result;

	result = malloc(sizeof(t_token) * ((ft_strlen(s) * 2) + 2));
	if (NULL == result)
		exit(EXIT_FAILURE);
	i = 0;
	while (i < (ft_strlen(s) * 2) + 1)
	{
		result[i] = (t_token){};
		result[i].status = NEVER_EXECUTED;
		result[i].value = NULL;
		i++;
	}
	result[i] = (t_token){};
	result[i].status = NEVER_EXECUTED;
	result[i].value = NULL;
	return (result);
}

void	pre_parse_and_execute(t_info *info)
{
	put_tree_level_marks(info);
	put_group_id_marks(info);
	put_tree_marks(info, info->tokens);
	if (!check_bad_syntax(info))
		parse_and_execute_tree(info);
}

void	parse_and_execute_string(char *s, t_info *info)
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
	result[k].type = EOF_TOKENS;
	info->tokens = result;
	info->num_of_tokens = k;
	pre_parse_and_execute(info);
}
