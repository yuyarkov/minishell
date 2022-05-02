/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tree_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/26 19:35:19 by dirony            #+#    #+#             */
/*   Updated: 2022/05/02 12:52:35 by dirony           ###   ########.fr       */
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
	i++;//чтобы не было бесконечного цикла
	while (i < info->num_of_tokens)
	{
		if (is_limiter(t[i]))
			return (&t[i]);
		i++;
	}
	return (NULL);
}

t_token	*get_next_root_limiter(t_token *token, t_info *info)//ищем следующий корень дерева
{
	t_token	*t;
	int		level;

	level = token->level;
	t = get_next_limiter(token, info);
	if (!t)
		return (NULL);
	while (t)
	{
		if (t->level == level)
			return (t);
		t = get_next_limiter(t, info);
	}
	return (NULL);
}

char	*get_command_from_token(t_token *t, t_info *info)
{
	int		group_id;
	int		i;
	char	*result;

	result = NULL;
	group_id = t->group_id;
	i = 0;
	while (t[i].type != END_OF_TOKENS && t[i].group_id == group_id && t[i].type != WORD)
		i++;
	if (t[i].type == WORD && t[i].group_id == group_id)
	{
		result = get_cmd_path(t[i].value, info->envp);//пока оставил вызов готовой костыльной функции с утечками
		t[i].type = CMD;
	}
	return (result);
}

char	**get_argv_from_token(t_token *t, t_info *info)
{
	int		group_id;
	int		i;
	int		k;
	char	**result;

	result = malloc(sizeof(char *) * (info->num_of_tokens + 1));
	group_id = t->group_id;
	i = 0;
	k = 0;
	while (t[i].type != END_OF_TOKENS && t[i].group_id == group_id)
	{
		if (t[i].type == WORD)
		{
			result[k] = t[i].value;
			t[i].type = ARGV;
			k++;
		}
		i++;
	}
	result[k] = NULL;
	return (result);
}

void	get_redirect_from_token(t_token *t, t_info *info, t_list *cmd)
{
	int		group_id;
	int		i;

	(void) info; //пока не пригодилось, может убрать аргумент
	group_id = t->group_id;
	i = 0;
	while (t[i].type != END_OF_TOKENS && t[i].group_id == group_id)
	{
		if (t[i].type == REDIRECT_IN)
		{
			if (t[i + 1].type == WORD)
			{
				t[i + 1].type = INPUT_FILE;//чтобы следующие парсеры не путались
				cmd->redirect_in = REDIRECT_IN;
				cmd->redirect_in_file = t[i].value;//тут нужно делать проверку на валидность файла
			}			
		}
		if (t[i].type == REDIRECT_OUT)
		{
			if (t[i + 1].type == WORD)
			{
				t[i + 1].type = OUTPUT_FILE;//чтобы следующие парсеры не путались
				cmd->redirect_out = REDIRECT_OUT;
				cmd->redirect_out_file = t[i].value;
			}	
		}
		i++;
	}
	//отдельной функцией добавить heredoc и append
}

t_list	*parse_token_group(t_token *t, t_info *info)
{
	int		group_id;
	int		i;
	t_list	*cmd;

	cmd = malloc(sizeof(t_list));
	if (NULL == cmd)
		exit(EXIT_FAILURE);
	group_id = t->group_id;
	i = 0;
	//здесь должна быть функция парсер, которая пройдётся по токенам и присвоит тип
	get_redirect_from_token(t, info, cmd);
	cmd->cmd = get_command_from_token(t, info);
	cmd->arguments = get_argv_from_token(t, info);
	
	// while (t[i].type != END_OF_TOKENS && t[i].group_id == group_id)
	// {
	// 	if (t[i].)
	// }	
				printf("parsing token group, command: %s\n", cmd->cmd);
				while (cmd->arguments[i])
				{
					printf("argv[%d]: %s\n", i, cmd->arguments[i]);
					i++;
				}

	return (cmd);
}

int	parse_and_execute_group(t_token *t, t_info *info)//для листьев дерева
{
	t_list	*cmd;

	if (t->status == NEVER_EXECUTED)
	{
		printf("=====executing group: %d, t.value: %s=====\n", t->group_id, t->value);
		cmd = parse_token_group(t, info);
			
	}
	else
		printf("======not executing group: %d, t.value: %s=====\n", t->group_id, t->value);
	t->status = 0;//подставить сюда исполнение
	return (t->status);
}

int	parse_and_execute_branch(t_token *t, t_info *info)//основная рекурсивная функция
{
	
	printf("=====executing branch: %d, t.type: %d=====\n", t->group_id, t->type);
	if (!t->left && !t->right)
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
		if (t)
			printf("next root limiter: %d\n", t->type);
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