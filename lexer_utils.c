/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/16 12:52:32 by dirony            #+#    #+#             */
/*   Updated: 2022/05/18 20:50:31 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	put_word_token(char *s, t_token *token, int *k)
{
	char	*word;
	int		i;

	word = malloc(ft_strlen(s) + 1);
	if (NULL == word)
		exit(EXIT_FAILURE);//выход из некорректного маллока
	i = 0;
	while (s[i] && !ft_strchr(SPECIAL_SYMBOLS, s[i]) && !ft_strchr(SPACES, s[i]))
	{
		word[i] = s[i];
		i++;
	}
	word[i] = '\0';
	token->type = WORD;
	token->value = word;
	*k = *k + 1;
	return (i);
}

int	has_double_special_symbol(char *s)
{
	if (ft_strncmp(s, "&&", 2) == 0)
		return (1);
	if (ft_strncmp(s, "||", 2) == 0)
		return (1);
	if (ft_strncmp(s, ">>", 2) == 0)
		return (1);
	if (ft_strncmp(s, "<<", 2) == 0)
		return (1);
	return (0);
}

int	put_double_special_token(char *s, t_token *token, int *k)
{
	*k = *k + 1;
	if (ft_strncmp(s, "&&", 2) == 0)
		token->type = AND_SIGN;
	if (ft_strncmp(s, "||", 2) == 0)
		token->type = OR_SIGN;
	if (ft_strncmp(s, ">>", 2) == 0)
		token->type = REDIRECT_APPEND;
	if (ft_strncmp(s, "<<", 2) == 0)
		token->type = REDIRECT_HEREDOC;	
	return (2);
}

int put_token_from_single_quotes(char *s, t_token *t, int *k)
{
	int		i;
	char	ch;
	char	*result;

	ch = s[0];
	result = malloc(ft_strlen(s) + 1);
	if (NULL == result)
		exit(EXIT_FAILURE);
	t[0].type = QUOTE;
	i = 1;
	while (s[i] && s[i] != ch)
	{
		result[i - 1] = s[i];
		i++;
	}
	result[i - 1] = '\0';
	i++;
	t[1].value = result;
	t[1].type = WORD;
	if (s[i - 1] == ch)
	{
		t[2].type = QUOTE;
		*k = *k + 3;
	}
	else
		*k = *k + 2;
	return (i);	
}

int put_token_from_double_quotes(char *s, t_token *t, int *k)
{
	int		i;
	int		j;
	char	ch;
	char	*result;

	ch = '\"';
	result = malloc(ft_strlen(s) + 1);
	if (NULL == result)
		exit(EXIT_FAILURE);
	i = 1;
	while (s[i] && s[i] != ch)
	{
		result = malloc(ft_strlen(s) + 1);
		if (NULL == result)
			exit(EXIT_FAILURE);
		j = 0;
		while (s[i] && s[i] != ch && s[i] != '$')
		{
			result[j] = s[i];
			i++;
			j++;
		}
		result[j] = '\0';
		t->value = result;
		t->type = WORD;
		t->inside_qoutes = 1;
		*k = *k + 1;
		t = &t[1];
		if (s[i] == '$')
		{
			t->inside_qoutes = 1;
			i = i + put_dollar_key_to_token(&s[i], t, k);
			*k = *k + 1;
			t = &t[1];
		}
	//	printf("inside put_token_from_double_qoutes, i: %d\n", i);
		//i++;
	}
	return (i + 1);	
}

int put_dollar_key_to_token(char *s, t_token *t, int *k)
{
	char	*key;
	int		i;

	key = malloc(ft_strlen(s) + 1);
	if (NULL == key)
		exit(EXIT_FAILURE);
	i = 1; // сам знак $ пропускаем и не будем записывать в токен
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

void	get_tokens_from_string(char *s, t_info *info)
{
	int		i;
	int		k;
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
	i = 0;
	k = 0;
	if (s[i])
	{
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
	}
	else
	{
		free(result);
		info->tokens = NULL;
		info->num_of_tokens = 0;
	}
}
