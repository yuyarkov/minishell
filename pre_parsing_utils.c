/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_parsing_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 20:57:31 by fdarkhaw          #+#    #+#             */
/*   Updated: 2022/05/29 13:12:19 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_dollar_value_from_env(char *s, t_info *info)
{
	t_env	*iter;
	char	*result;

	if (*s == '?')
		return (ft_itoa(info->status));
	result = NULL;
	iter = info->env;
	while (iter)
	{
		if (ft_strncmp(iter->key, s, ft_strlen(iter->key)) == 0)
			result = ft_strdup(iter->value);
		iter = iter->next;
	}
	return (result);
}

void	check_and_replace_dollar(t_token *t, t_info *info)
{
	int		group;
	int		i;
	char	*temp;

	group = t->group;
	i = 0;
	while (t && t[i].type != EOF_TOKENS && t[i].group == group)
	{
		if (t[i].type == DOLLAR_KEY)
		{
			temp = t[i].value;
			t[i].value = get_dollar_value_from_env(t[i].value, info);
			t[i].type = WORD;
			free(temp);
		}
		i++;
	}
}

char	*join_word_chain(t_token *t, int *inc)
{
	int		gr;
	int		i;	
	char	*result;
	char	*temp;

	gr = t->group;
	result = NULL;
	i = *inc;
	while (t && t[i].in_qoutes && t[i].type != EOF_TOKENS && t[i].group == gr)
	{
		temp = result;
		result = ft_strjoin(result, t[i].value);
		free(temp);
		t[i].type = ARGV;
		i++;
	}
	*inc = i;
	return (result);
}

void	join_words_inside_quotes(t_token *t)
{
	int		gr;
	int		i;
	int		k;
	char	*result;
	char	*temp;

	gr = t->group;
	i = 0;
	result = t[i].value;
	while (t && t[i].type != EOF_TOKENS && t[i].group == gr)
	{
		while (!t[i].in_qoutes && t[i].type != EOF_TOKENS && t[i].group == gr)
			i++;
		if (t[i].in_qoutes && t[i].type != EOF_TOKENS && t[i].group == gr)
		{
			k = i;
			temp = t[k].value;
			t[k].value = join_word_chain(t, &i);
			t[k].type = WORD;
			free(temp);
		}
	}
}
