/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 20:35:36 by dirony            #+#    #+#             */
/*   Updated: 2021/10/16 14:24:47 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(char const *s, char ch)
{
	int	i;
	int	flag;
	int	word_count;

	i = 0;
	flag = 1;
	word_count = 0;
	while (s[i])
	{
		if (s[i] == ch)
			flag = 1;
		if ((s[i] != ch) && flag)
		{
			flag = 0;
			word_count++;
		}
		i++;
	}
	return (word_count);
}

static char	*allocate_one_word(char **result, int w, int size)
{
	result[w] = malloc(size + 2);
	if (NULL == result[w])
	{
		while (w)
			free(result[w--]);
		free(result);
		return (NULL);
	}
	return (result[w]);
}

static char	**allocate_words(char const *s, char ch, char **result, int w_count)
{
	int	i;
	int	flag;
	int	w;
	int	letters;

	i = 0;
	w = 0;
	flag = 1;
	letters = 0;
	while (w < w_count)
	{
		while (s[i++] == ch)
			letters = 0;
		while (s[i] && (s[i++] != ch))
			letters++;
		result[w] = allocate_one_word(result, w, letters);
		if (NULL == result[w])
			return (NULL);
		w++;
	}
	return (result);
}

static void	put_words(char const *s, int ch, char **result, int word_count)
{
	int		i;
	int		j;
	int		flag;
	int		w;

	i = 0;
	w = 0;
	flag = 1;
	while (w < word_count)
	{
		while (s[i++] == ch)
			flag = 1;
		i--;
		while ((s[i] != ch) && s[i])
		{
			if (flag)
			{
				flag = 0;
				j = 0;
			}
			result[w][j++] = s[i++];
		}
		result[w++][j] = '\0';
	}
}

char	**ft_split(char const *s, char ch)
{
	int		word_count;
	char	**result;

	if (!s)
		return (NULL);
	word_count = count_words(s, ch);
	result = malloc(sizeof(char *) * (word_count + 1));
	if (NULL == result)
		return (NULL);
	result = allocate_words(s, ch, result, word_count);
	if (NULL == result)
		return (NULL);
	put_words(s, ch, result, word_count);
	result[word_count] = NULL;
	return (result);
}
