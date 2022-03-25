/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 15:53:52 by fdarkhaw          #+#    #+#             */
/*   Updated: 2022/03/25 21:02:19 by fdarkhaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sorting_and_print_env(t_env *env)
{
	(void) env;
}

t_env	*export_create_elem(char *str)
{
	t_env	*new_elem;
	char	**result;

	new_elem = malloc(sizeof(t_env));
	if (NULL == new_elem)
		return (NULL);
	new_elem->next = NULL;
	result = ft_split(str, '=');
	new_elem->key = ft_substr(result[0], 0, ft_strlen(result[0]));
	new_elem->value = ft_substr(result[1], 0, ft_strlen(result[1]));
	free_str_pointer(result);
	return (new_elem);
}

int	find_argument_in_env(char *argument, t_env **env)
{
	char	**split_argument;
	int		return_strncmp;
	char	*return_strchr;
	t_env	*tmp;

	tmp = *env;
	split_argument = ft_split(argument, '=');
	while (tmp)
	{
		return_strncmp = ft_strncmp(tmp->key, split_argument[0], ft_strlen(tmp->key));
		return_strchr = ft_strchr(argument, '=');
		if (!return_strncmp && return_strchr)//аргумент уже есть в енв и подан С =
		{
			free(tmp->value);
			if(split_argument[1])
				tmp->value = ft_substr(split_argument[1], 0, ft_strlen(split_argument[1]));
			else
				tmp->value = ft_substr("\0", 0, 1);
			return (1);
		}
		else if (!return_strncmp && !return_strchr)//аргумент уже есть в енв и подан БЕЗ =
			return (1);
		tmp = tmp->next;
	}
	free_str_pointer(split_argument);
	return (0);
}

int	parser_arguments(t_list *cmd, t_env **env)
{
	int		iterator;
	int		result;
	// t_env	*new_elem;

	result = 0;
	iterator = 1;
	while (cmd->arguments[iterator])
	{
		if (ft_isalpha(cmd->arguments[iterator][0])	|| cmd->arguments[iterator][0] == '_')// аргум начинается с буквы или _
		{
			if (!find_argument_in_env(cmd->arguments[iterator], env))
				env_lstadd_back(env, env_create_elem(cmd->arguments[iterator]));
			// {
			// 	new_elem = env_create_elem(cmd->arguments[iterator]);
			// 	if (!new_elem)//если память не выделена добавить обработку ошибки
			// 		printf("new_elem == NULL, return ERROR\n");//perror()? нужна ли эта проверка?
			// 	env_lstadd_back(env, new_elem);
			// }
		}
		else
		{
			result = 1;
			ft_putstr_fd("minishell: export: '", 2);
			ft_putstr_fd(cmd->arguments[iterator], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
		}
		iterator++;
	}
	return (result);
}

int	execute_export_command(t_list *cmd, char **envp, t_env *env)
{
	int	result;

	(void)envp;
	result = 0;
	if (cmd->arguments[1])
		result = parser_arguments(cmd, &env);// done
	else
		sorting_and_print_env(env);//next challenge
	return (result);
}
