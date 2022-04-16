/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/16 12:52:32 by dirony            #+#    #+#             */
/*   Updated: 2022/04/16 22:07:23 by fdarkhaw         ###   ########.fr       */
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

int	put_special_token(char *s, t_token *token)
{
	token->value = NULL;
	if (has_double_special_symbol(s))
		return put_double_special_token(s, token);
	if (*s == '|')
		token->type = PIPE;
	if (*s == '\'')
		token->type = QOUTE;
	if (*s == '\"')
		token->type = DOUBLE_QOUTE;
	if (*s == '(')
		token->type = LEFT_PARENTHESIS;
	if (*s == ')')
		token->type = RIGHT_PARENTHESIS;
	if (*s == '\\')
		token->type = BACKSLASH;
	if (*s == '$')
		token->type = DOLLAR_SIGN;
	return (1);
}

void	get_tokens_from_string(char *s, t_info *info)
{
	int		i;
	int		k;
	t_token	*result;

	result = malloc(sizeof(t_token) * ft_strlen(s));
	if (NULL == result)
		exit(EXIT_FAILURE);//Юра - выход из некорректного маллока
	i = 0;
	k = 0;
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
	info->tokens = result;
	info->num_of_tokens = k;
}