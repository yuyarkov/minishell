/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/16 12:52:32 by dirony            #+#    #+#             */
/*   Updated: 2022/05/27 20:20:36 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	put_word_token(char *s, t_token *token, int *k)
{
	char	*word;
	int		i;

	word = malloc(ft_strlen(s) + 1);
	if (NULL == word)
		exit(EXIT_FAILURE);
	i = 0;
	while (s[i] && !ft_strchr(SPECIAL_SYMBOLS, s[i])
		&& !ft_strchr(SPACES, s[i]))
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

int	put_token_from_single_quotes(char *s, t_token *t, int *k)
{
	int		i;
	char	*result;

	result = malloc(ft_strlen(s) + 1);
	if (NULL == result)
		exit(EXIT_FAILURE);
	t[0].type = QUOTE;
	i = 1;
	while (s[i] && s[i] != '\'')
	{
		result[i - 1] = s[i];
		i++;
	}
	result[i - 1] = '\0';
	t[1].value = result;
	t[1].type = WORD;
	if (s[i] == '\'')
	{
		t[2].type = QUOTE;
		*k = *k + 3;
		return (i + 1);
	}
	else
		*k = *k + 2;
	return (i);
}
