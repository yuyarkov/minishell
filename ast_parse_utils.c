/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_parse_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 19:03:58 by dirony            #+#    #+#             */
/*   Updated: 2022/05/28 16:47:48 by fdarkhaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_limiter(t_token t)
{
	if (t.type == AND_SIGN || t.type == OR_SIGN)
		return (1);
	else
		return (0);
}

void	put_tree_level_marks(t_info *info)
{
	t_token	*tokens;
	int		i;
	int		level;

	tokens = info->tokens;
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
	put_group_id_marks(info);
	put_tree_marks(info, info->tokens);
}

void	put_group_id_marks(t_info *info)
{
	t_token	*t;
	int		i;
	int		group;

	t = info->tokens;
	i = 0;
	group = 0;
	while (t && i < info->num_of_tokens)
	{
		if (is_limiter(t[i]))
			group++;
		if (t[i].type != LEFT_PARENTHESIS && t[i].type != RIGHT_PARENTHESIS)
			t[i].group = group;
		else
			t[i].group = -1;
		if (is_limiter(t[i]))
			group++;
		i++;
	}	
}

t_token	*get_group_start_point(int group, t_info *info)
{
	t_token	*t;
	int		i;

	t = info->tokens;
	i = 0;
	while (i < info->num_of_tokens)
	{
		if (t[i].group == group)
			return (&t[i]);
		i++;
	}
	return (NULL);
}

void	put_tree_marks(t_info *info, t_token *t)
{
	t_token	*next;
	int		i;

	i = -1;
	while (t && ++i < info->num_of_tokens)
	{
		if (is_limiter(t[i]))
		{
			t[i].left = get_group_start_point(t[i].group - 1, info);
			t[i].right = get_group_start_point(t[i].group + 1, info);
		}
	}
	i = -1;
	while (t && ++i < info->num_of_tokens)
	{
		if (is_limiter(t[i]))
		{
			next = get_next_limiter(&t[i], info);
			if (next && next->level > t[i].level)
				t[i].right = next;
			if (next && next->level < t[i].level)
				next->left = &t[i];
		}
	}
}
