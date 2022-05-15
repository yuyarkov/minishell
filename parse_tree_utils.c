/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tree_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/26 19:35:19 by dirony            #+#    #+#             */
/*   Updated: 2022/05/15 18:21:57 by dirony           ###   ########.fr       */
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
	while (t && t[i].type != END_OF_TOKENS)
	{
		if (t[i].type == AND_SIGN || t[i].type == OR_SIGN)
			result = t[i].level;
		i++;		
	}
	while (t && t[i].type != END_OF_TOKENS)
	{
		if ((t[i].type == AND_SIGN || t[i].type == OR_SIGN) && t[i].level < result)
			result = t[i].level;
		i++;
	}
	return (result);	
}

t_token	*get_next_root_limiter(t_token *token, t_info *info)//ищем следующий корень дерева
{
	t_token	*t;
	int		level;

	level = get_min_level(token);
	t = get_next_limiter(token, info);
	if (!t)
		return (NULL);
	while (t)
	{
		if (t->level == level && t->status == NEVER_EXECUTED)//убедиться, что у корневого элемента уровень всегда 0
			return (t);
		t = get_next_limiter(t, info);
	}
	return (NULL);
}

void	get_command_from_token(t_token *t, t_info *info, t_list *cmd)
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
		result = get_cmd_path(t[i].value, info->envp, info);//пока оставил вызов готовой костыльной функции с утечками
		t[i].type = CMD;
	}
	cmd->cmd = result;
}

void	check_and_replace_dollar(t_token *t, t_info *info)
{
	int		group_id;
	int		i;

	group_id = t->group_id;
	i = 0;
	while (t && t[i].type != END_OF_TOKENS && t[i].group_id == group_id)
	{
		if (t[i].type == DOLLAR_KEY)
		{
			//free(t[i].value);//здесь не надо освобождать, а где?
			t[i].value = get_dollar_value_from_env(t[i].value, info);
			t[i].type = WORD;
		}
		i++;
	}
}

void	join_words_inside_quotes(t_token *t)
{
	int		group_id;
	int		i;
	int		k;
	char	*result;

	group_id = t->group_id;
	i = 0;
	result = NULL;
	while (t && t[i].type != END_OF_TOKENS && t[i].group_id == group_id)
	{
		while (!t[i].inside_qoutes && t[i].type != END_OF_TOKENS && t[i].group_id == group_id)
			i++;
		if (t[i].inside_qoutes && t[i].type != END_OF_TOKENS && t[i].group_id == group_id)
		{
			k = i;
			result = t[i].value;
			i++;
			while (t[i].inside_qoutes && t[i].type != END_OF_TOKENS && t[i].group_id == group_id)
			{
				result = ft_strjoin(result, t[i].value);//сделать очистку утечек через temp
				t[i].type = ARGV; //чтобы больше не попадало в аргументы
				i++;
			}
			t[k].value = result;
		}
	}
}

void	get_argv_from_token(t_token *t, t_info *info, t_list *cmd)
{
	int		group_id;
	int		i;
	int		k;
	char	**result;

	check_and_replace_dollar(t, info);
	//check_and_replace_asterisk(t, info); //замена звёздочки на строку
	join_words_inside_quotes(t);
	result = malloc(sizeof(char *) * (info->num_of_tokens + 1));
	if (NULL == result)
		exit(EXIT_FAILURE);
	group_id = t->group_id;
	result[0] = ft_strdup(cmd->cmd);
	if (t && t->type == PIPE)
		i = 1;
	else
		i = 0;
	k = 1;
	while (t && t[i].type != END_OF_TOKENS && t[i].group_id == group_id && t[i].type != PIPE)
	{
		if (t[i].type == WORD && t[i].value && *t[i].value != '\0')
		{
			result[k] = t[i].value;
			t[i].type = ARGV;
			k++;
		}
		i++;
	}
	if (t && t[i].type == PIPE)
		cmd->limiter = PIPE;
	if (result[0])
		result[k] = NULL;
	cmd->arguments = result;
}

void	get_redirect_from_token(t_token *t, t_info *info, t_list *cmd)
{
	int		group_id;
	int		i;

	(void) info; //пока не пригодилось, может убрать аргумент
	group_id = t->group_id;
	if (t[0].type == PIPE)
		i = 1;
	else
		i = 0;
	while (t[i].type != END_OF_TOKENS && t[i].group_id == group_id && t[i].type != PIPE)
	{
		if (t[i].type == REDIRECT_IN)
		{
			if (t[i + 1].type == WORD)
			{
				t[i + 1].type = INPUT_FILE;
				cmd->redirect_in = t[i].type;
				cmd->redirect_in_file = t[i + 1].value;//тут нужно делать проверку на валидность файла
				//printf("inside parser, redirect_in_file: %s\n", cmd->redirect_in_file);
			}			
		}
		if (t[i].type == REDIRECT_HEREDOC)
		{
			if (t[i + 1].type == WORD)
			{
				t[i + 1].type = HEREDOC_EOF;
				cmd->redirect_in = t[i].type;
				cmd->heredoc_eof = t[i + 1].value;
			}			
		}
		if (t[i].type == REDIRECT_OUT || t[i].type == REDIRECT_APPEND)
		{
			if (t[i + 1].type == WORD)
			{
				t[i + 1].type = OUTPUT_FILE;
				cmd->redirect_out = t[i].type;
				cmd->redirect_out_file = t[i + 1].value;
				cmd->fd[1] = open(cmd->redirect_out_file, O_CREAT | O_RDWR | O_TRUNC, 0644);
				close(cmd->fd[1]);
			}	
		}
		i++;
	}
}

t_list	*create_elem_cmd(t_token *t, t_info *info)
{
	t_list	*cmd;

	cmd = malloc(sizeof(t_list));
	if (NULL == cmd)
		exit(EXIT_FAILURE);
	*cmd = (t_list){};
	get_redirect_from_token(t, info, cmd);
	get_command_from_token(t, info, cmd);
	get_argv_from_token(t, info, cmd);
	return (cmd);
}

t_list	*parse_token_group(t_token *t, t_info *info)
{
	int		group_id;
	int		i;
	t_list	*temp;
	t_list	*first_elem;

	first_elem = create_elem_cmd(t, info);
	i = 0;
	group_id = t->group_id;
	while (t[i].type != END_OF_TOKENS && t[i].group_id == group_id)
	{
		while (t[i].type != END_OF_TOKENS && t[i].group_id == group_id && t[i].type != PIPE)
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
				// 				printf("parsing token group, command: %s\n", temp->cmd);
				// 				i = 0;
				// 				while (temp->arguments && temp->arguments[i])
				// 				{
				// 					printf("argv[%d]: %s\n", i, temp->arguments[i]);
				// 					i++;
				// 				}
				// 	temp = temp->next;
				// }
	return (first_elem);
}

int	parse_and_execute_group(t_token *t, t_info *info)//для листьев дерева
{
	t_list	*cmd;
	int		i;

	cmd = NULL;
	if (t->status == NEVER_EXECUTED)
	{
	//	printf("=====executing group: %d, t.value: %s=====\n", t->group_id, t->value);
		i = 0;
		while (t[i].type == LEFT_PARENTHESIS)//проматываю левые скобки в начале группы
			i++;
		cmd = parse_token_group(&t[i], info);
		info->commands = cmd;			
	}
	else
		return (t->status);
	t->status = execute_group(cmd, info->envp, info);//подставить сюда исполнение
	return (t->status);
}

int	parse_and_execute_branch(t_token *t, t_info *info)//основная рекурсивная функция
{
	
	//printf("=====executing branch: %d, t.type: %d, t.status: %d=====\n", t->group_id, t->type, t->status);
	if (!t->left && !t->right)
		return (parse_and_execute_group(t, info));
	if (t->status == NEVER_EXECUTED)//защита от повторного обхода дерева слева от другого корня
		t->status = parse_and_execute_branch(t->left, info);
	if ((t->status == 0 && t->type == AND_SIGN) ||
			(t->status != 0 && t->type == OR_SIGN))
		t->status = parse_and_execute_branch(t->right, info);
	//printf("inside parse_and_execute_branch, t->status: %d\n", t->status);
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