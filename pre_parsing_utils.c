/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_parsing_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 20:57:31 by fdarkhaw          #+#    #+#             */
/*   Updated: 2022/05/05 22:11:41 by fdarkhaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
