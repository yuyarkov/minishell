/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tree_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/26 19:35:19 by dirony            #+#    #+#             */
/*   Updated: 2022/04/27 21:06:42 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*get_next_limiter(t_token *token, t_info *info)//здесь порочная практика с поиском от индекса. Надо потом сдвинутый индекс как-то передать наверх
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

int	parse_and_execute_group(t_token *t)//для листьев дерева
{
	if (t->status == NEVER_EXECUTED)
		printf("=====executing group: %d, t.value: %s=====\n", t->group_id, t->value);
	else
		printf("======not executing group: %d, t.value: %s=====\n", t->group_id, t->value);
	t->status = 0;//подставить сюда исполнение
	return (t->status);
}

int	parse_and_execute_branch(t_token *t)//основная рекурсивная функция
{
	
	printf("=====executing branch: %d, t.type: %d=====\n", t->group_id, t->type);
	if (!t->left && !t->right)
		return (parse_and_execute_group(t));
	if (t->status == NEVER_EXECUTED)//защита от повторного обхода дерева слева от другого корня
		t->status = parse_and_execute_branch(t->left);
	if ((t->status == 0 && t->type == AND_SIGN) ||
			(t->status != 0 && t->type == OR_SIGN))
		t->status = parse_and_execute_branch(t->right);
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
			return (parse_and_execute_group(info->tokens));//вырожденный случай, когда нет лимитеров в строке
		if (t)
			parse_and_execute_branch(t);
	}
	if (t)
		return (t->status);
	else
		return (EXIT_FAILURE);//вопрос, что возвращать в этом случае
}