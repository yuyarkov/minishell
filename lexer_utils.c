/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/16 12:52:32 by dirony            #+#    #+#             */
/*   Updated: 2022/04/16 17:45:03 by dirony           ###   ########.fr       */
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

int	put_special_token(char *s, t_token *token)
{
	token->value = NULL;
	
	if (*s == '\'')
		token->type = QOUTE;
	if (*s == '|')
		token->type = PIPE;

	return (1);//выдавать либо 1 либо 2 в зависимости от длины спецкоманды
}

void	get_tokens_from_string(char *s, t_info *info)
{
	int		i;
	int		k;
	t_token	*result;
	
	result = malloc(sizeof(t_token) * ft_strlen(s));
	if (NULL == result)
		exit(EXIT_FAILURE);//выход из некорректного маллока
	i = 0;
	k = 0;
	while (s[i])
	{
		while (ft_strchr(SPACES, s[i]))
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

void	clear_tokens(t_info *info)
{
	int	i;

	i = 0;
	while (i < info->num_of_tokens)
	{
		free(info->tokens[i].value);
		i++;
	}
	free(info->tokens);
}

void	print_tokens(t_info *info)
{
	int	i;

	printf("Num of tokens: %d\n", info->num_of_tokens);
	i = 0;
	while (i < info->num_of_tokens)
	{
		printf("token %d\ttype: %d,\tvalue: %s\n", i, info->tokens[i].type, info->tokens[i].value);
		i++;
	}
}