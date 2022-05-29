/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tree_utils2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/27 20:13:31 by dirony            #+#    #+#             */
/*   Updated: 2022/05/29 13:11:26 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*parse_token_group(t_token *t, t_info *info)
{
	int		group;
	int		i;
	t_list	*temp;
	t_list	*first_elem;

	first_elem = create_elem_cmd(t, info);
	i = 0;
	group = t->group;
	while (t[i].type != EOF_TOKENS && t[i].group == group)
	{
		while (t[i].type != EOF_TOKENS && t[i].group == group
			&& t[i].type != PIPE)
			i++;
		if (t[i].type == PIPE)
		{
			temp = create_elem_cmd(&t[i], info);
			ft_double_list_add_back(&first_elem, temp);
			i++;
		}
	}
	return (first_elem);
}

int	parse_and_execute_group(t_token *t, t_info *info)
{
	t_list	*cmd;
	int		i;

	cmd = NULL;
	if (t->status == NEVER_EXECUTED)
	{
		i = 0;
		while (t[i].type == LEFT_PARENTHESIS)
			i++;
		cmd = parse_token_group(&t[i], info);
		info->commands = cmd;
	}
	else
		return (t->status);
	t->status = execute_group(cmd, info->envp, info);
	clear_cmd(cmd);
	return (t->status);
}

int	mark_branch_not_executable(t_token *t)
{
	if (t->left)
		mark_branch_not_executable(t->left);
	if (t->right)
		mark_branch_not_executable(t->right);
	t->status = 0;
	return (0);
}

int	parse_and_execute_branch(t_token *t, t_info *info)
{
	if (!t->left && !t->right)
		return (parse_and_execute_group(t, info));
	if (t->status == NEVER_EXECUTED)
		t->status = parse_and_execute_branch(t->left, info);
	if ((t->status == 0 && t->type == AND_SIGN)
		|| (t->status != 0 && t->type == OR_SIGN))
		t->status = parse_and_execute_branch(t->right, info);
	if (t->status == 0 && t->type == OR_SIGN)
		mark_branch_not_executable(t);
	return (t->status);
}

int	parse_and_execute_tree(t_info *info)
{
	t_token	*t;

	t = info->tokens;
	while (t)
	{
		t = get_next_root_limiter(t, info);
		if (!t)
			return (parse_and_execute_group(info->tokens, info));
		if (t)
			parse_and_execute_branch(t, info);
	}
	if (t)
		return (t->status);
	else
		return (EXIT_FAILURE);
}
