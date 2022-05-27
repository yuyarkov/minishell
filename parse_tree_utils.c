/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tree_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/26 19:35:19 by dirony            #+#    #+#             */
/*   Updated: 2022/05/27 21:51:33 by fdarkhaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*get_next_limiter(t_token *token, t_info *info)
{
	t_token	*t;
	int		i;

	t = info->tokens;
	i = 0;
	while (&t[i] != token)
		i++;
	i++;
	while (i < info->num_of_tokens)
	{
		if (is_limiter(t[i]))
			return (&t[i]);
		i++;
	}
	return (NULL);
}

int	get_min_level(t_token *t)
{
	int		result;
	int		i;

	if (!t)
		return (0);
	i = 0;
	result = 0;
	while (t && t[i].type != EOF_TOKENS)
	{
		if (t[i].type == AND_SIGN || t[i].type == OR_SIGN)
			result = t[i].level;
		i++;
	}
	i = 0;
	while (t && t[i].type != EOF_TOKENS)
	{
		if ((t[i].type == AND_SIGN || t[i].type == OR_SIGN)
			&& t[i].level < result)
			result = t[i].level;
		i++;
	}
	return (result);
}

t_token	*get_next_root_limiter(t_token *token, t_info *info)
{
	t_token	*t;
	int		level;

	level = get_min_level(token);
	t = get_next_limiter(token, info);
	if (!t)
		return (NULL);
	while (t)
	{
		if (t->level == level && t->status == NEVER_EXECUTED)
			return (t);
		t = get_next_limiter(t, info);
	}
	return (NULL);
}

void	get_command_from_token(t_token *t, t_info *info, t_list *cmd)
{
	int		group;
	int		i;
	char	*result;

	result = NULL;
	group = t->group;
	i = 0;
	while (t[i].type != EOF_TOKENS && t[i].group == group && t[i].type != WORD)
		i++;
	if (t[i].type == WORD && t[i].group == group && t[i].type != EOF_TOKENS)
	{
		result = get_cmd_path(t[i].value, info);
		t[i].type = CMD;
	}
	cmd->cmd = result;
	if (!result)
		print_cmd_error(t[i].value, &info->status);
}

t_list	*create_elem_cmd(t_token *t, t_info *info)
{
	t_list	*cmd;

	cmd = malloc(sizeof(t_list));
	if (NULL == cmd)
		exit(EXIT_FAILURE);
	*cmd = (t_list){};
	cmd->cmd = NULL;
	get_redirect_from_token(t, cmd);
	get_command_from_token(t, info, cmd);
	if (cmd->cmd)
		get_argv_from_token(t, info, cmd, t->group);
	return (cmd);
}
