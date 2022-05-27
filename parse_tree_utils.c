/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tree_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/26 19:35:19 by dirony            #+#    #+#             */
/*   Updated: 2022/05/26 22:29:11 by fdarkhaw         ###   ########.fr       */
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
	while (t && t[i].type != EOF_TOKENS)
	{
		if (t[i].type == AND_SIGN || t[i].type == OR_SIGN)
			result = t[i].level;
		i++;
	}
	while (t && t[i].type != EOF_TOKENS)
	{
		if ((t[i].type == AND_SIGN || t[i].type == OR_SIGN) && t[i].level < result)
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
		result = get_cmd_path(t[i].value, info->envp, info);
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
		while (t[i].type != EOF_TOKENS && t[i].group == group && t[i].type != PIPE)
			i++;
		if (t[i].type == PIPE)
		{
			temp = create_elem_cmd(&t[i], info);
			ft_double_list_add_back(&first_elem, temp);
			i++;
		}
	}
	//debug
				// temp = first_elem;
				// while (temp)
				// {
				// 				printf("parsing token group, command: %s, cmd: %p\n", temp->cmd, temp);
				// 				i = 0;
				// 				while (temp->arguments && temp->arguments[i])
				// 				{
				// 					printf("argv[%d]: %s\n", i, temp->arguments[i]);
				// 					i++;
				// 				}
				// 				printf("redirect_in: %s\n", temp->redirect_in_file);
				// 				//printf("redirect_out: %s\n", temp->redirect_out_file);
				// 	temp = temp->next;
				// }
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
	return (t->status);
}

int	parse_and_execute_branch(t_token *t, t_info *info)//основная рекурсивная функция
{
	
	// printf("=====executing branch: %d, t.type: %d, t.status: %d=====\n", t->group, t->type, t->status);
	if (!t->left && !t->right)//конечный случай рекурсии
		return (parse_and_execute_group(t, info));
	if (t->status == NEVER_EXECUTED)//защита от повторного обхода дерева слева от другого корня
		t->status = parse_and_execute_branch(t->left, info);
	if ((t->status == 0 && t->type == AND_SIGN) ||
			(t->status != 0 && t->type == OR_SIGN))
		t->status = parse_and_execute_branch(t->right, info);
	return (t->status);
}

int	parse_and_execute_tree(t_info *info)
{
	t_token *t;

	t = info->tokens;
	while (t)//основной большой цикл.
	{
		t = get_next_root_limiter(t, info);
		// if (t)
		// 	printf("next root limiter: %d\n", t->type);
		if (!t)
			return (parse_and_execute_group(info->tokens, info));//вырожденный случай, когда нет лимитеров в строке
		if (t)
			parse_and_execute_branch(t, info);
	}
	if (t)
		return (t->status);
	else
		return (EXIT_FAILURE);//вопрос, что возвращать в этом случае
}
