/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_parse_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 19:03:58 by dirony            #+#    #+#             */
/*   Updated: 2022/04/22 20:01:23 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_limiter(t_token t)
{
	if (t.type == LEFT_PARENTHESIS || t.type == RIGHT_PARENTHESIS
			|| t.type == AND_SIGN || t.type == OR_SIGN || t.type == PIPE)
		return (1);
	else
		return (0);
}

void	put_tree_level_marks(t_info *info)//подумать нужен ли int или void
{
	t_token	*tokens;
	int		i;
	int		level;
	
	tokens = info->tokens;//проверку на пустоту не делаю, до этого по идее всё проверили
	i = 0;
	level = 0;
	while (tokens && i < info->num_of_tokens)
	{
		if (tokens[i].type == LEFT_PARENTHESIS)
			level++;
		if (tokens[i].type == RIGHT_PARENTHESIS)
			level--;
		tokens[i].level = level;
		i++;		
	}
}

void	put_group_id_marks(t_info *info)
{
	t_token	*tokens;
	int		i;
	int		group_id;
	
	tokens = info->tokens;
	i = 0;
	group_id = 0;
	while (tokens && i < info->num_of_tokens)
	{
		if (is_limiter(tokens[i]))
			group_id++;
		tokens[i].group_id = group_id;
		i++;
	}	
}