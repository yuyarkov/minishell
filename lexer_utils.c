/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/16 12:52:32 by dirony            #+#    #+#             */
/*   Updated: 2022/05/08 17:15:47 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	put_word_token(char *s, t_token *token)
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

int	put_double_special_token(char *s, t_token *token)
{
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

int put_token_from_quotes(char *s, t_token *t)//не забыть поменять тип токена, вернуть число пройденных символов
{
	int		i;
	char	ch;
	char	*result;

	ch = s[0];
	result = malloc(ft_strlen(s) + 1);
	if (NULL == result)
		exit(EXIT_FAILURE);
	i = 1;
	while (s[i] && s[i] != ch)
	{
		result[i - 1] = s[i];
		i++;
	}
	result[i - 1] = '\0';
	i++;
	t->value = result;
	t->type = WORD;
	return (i);	
}

int	put_special_token(char *s, t_token *token)
{
	token->value = NULL;
	if (has_double_special_symbol(s))
		return (put_double_special_token(s, token));
	if (*s == '|')
		token->type = PIPE;
	if (*s == '\'' || *s == '\"')
		return (put_token_from_quotes(s, token));
	if (*s == '(')
		token->type = LEFT_PARENTHESIS;
	if (*s == ')')
		token->type = RIGHT_PARENTHESIS;
	if (*s == '\\')
		token->type = BACKSLASH;
	if (*s == '$')
		token->type = DOLLAR_SIGN;
	if (*s == '>')
		token->type = REDIRECT_OUT;
	if (*s == '<')
		token->type = REDIRECT_IN;
	return (1);
}

void	get_tokens_from_string(char *s, t_info *info)
{
	int		i;
	int		k;
	t_token	*result;

	result = malloc(sizeof(t_token) * ft_strlen(s) + 1);
	if (NULL == result)
		exit(EXIT_FAILURE);//Юра - выход из некорректного маллока
	i = 0;
	while (s[i])
	{
		result[i] = (t_token){};//зануляет структуру - подсказка от Николая
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
					i = i + put_word_token(&s[i], &result[k++]);
				else
					i = i + put_special_token(&s[i], &result[k++]);
			}
		}
		result[k].type = END_OF_TOKENS;
		info->tokens = result;
		info->num_of_tokens = k;
	}
	else
	{
		info->tokens = NULL;
		info->num_of_tokens = 0;
	}
}
