/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_parsing_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 20:57:31 by fdarkhaw          #+#    #+#             */
/*   Updated: 2022/05/11 19:16:39 by dirony           ###   ########.fr       */
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
		if (ft_strncmp(iter->key, s, ft_strlen(s)) == 0)
			result = ft_strdup(iter->value);
		iter = iter->next;
	}
	return (result);
}


int	dollar_processing(t_info *info)
{
	int	i;

	i = 0;
	while (i < info->num_of_tokens)
	{
		if (info->tokens[i].type == DOLLAR_SIGN)
		{
			printf("%s\n", info->tokens[i + 1].value);
			if (!ft_strncmp(info->tokens[i + 1].value, "?\0", ft_strlen(info->tokens[i + 1].value)))
				printf("HERE 2\n");//вместо ? положить info.status
		}
		i++;
	}
	return (0);
}
