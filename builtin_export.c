/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 15:53:52 by fdarkhaw          #+#    #+#             */
/*   Updated: 2022/03/29 12:55:56 by fdarkhaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_export(t_env *env)
{
	while (env)
	{
		if (env->value)
		{
			ft_putstr_fd("declare -x ", 1);
			ft_putstr_fd(env->key, 1);
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(env->value, 1);
			ft_putendl_fd("\"", 1);
		}
		else
		{
			ft_putstr_fd("declare -x ", 1);
			ft_putendl_fd(env->key, 1);
		}
		env = env->next;
	}
}

t_env	*export_create_elem(char *str)
{
	t_env	*new_elem;
	char	**result;

	new_elem = malloc(sizeof(t_env));// может вернуть NULL
	if (NULL == new_elem)
		return (NULL);
	new_elem->next = NULL;
	// printf("str = %s\n", str);
	if (ft_strchr(str, '='))//если есть =
	{
		result = ft_split(str, '=');// может вернуть NULL
		new_elem->key = ft_substr(result[0], 0, ft_strlen(result[0]));//substr может вернуть NULL
		if (!result[1])// нет value
			new_elem->value = ft_substr("\0", 0, 1);//substr может вернуть NULL
		else// есть value
			new_elem->value = ft_substr(result[1], 0, ft_strlen(result[1]));//substr может вернуть NULL
		free_str_pointer(result);
	}
	else//если нет =
	{
		new_elem->key = ft_substr(str, 0, ft_strlen(str));//substr может вернуть NULL
		new_elem->value = NULL;
	}
	return (new_elem);
}

int	find_argument_in_env(char *argument, t_env **env)
{
	char	**split_argument;
	int		return_strncmp;
	char	*return_strchr;
	t_env	*tmp;
	int		result;
	int		max_strlen;

	tmp = *env;
	split_argument = ft_split(argument, '=');//split может вернуть NULL
	result = 0;
	while (tmp)
	{
		max_strlen = find_max_strlen(split_argument[0], tmp->key);
		return_strncmp = ft_strncmp(split_argument[0], tmp->key, max_strlen);
		return_strchr = ft_strchr(argument, '=');
		if (!return_strncmp && return_strchr)//аргумент уже есть в енв и подан С =
		{
			free(tmp->value);
			if(split_argument[1])
				tmp->value = ft_substr(split_argument[1], 0, ft_strlen(split_argument[1]));//substr может вернуть NULL
			else
				tmp->value = ft_substr("\0", 0, 1);//substr может вернуть NULL
			result = 1;
		}
		else if (!return_strncmp && !return_strchr)//аргумент уже есть в енв и подан БЕЗ =
			result = 1;
		tmp = tmp->next;
	}
	free_str_pointer(split_argument);
	return (result);
}

int	add_arguments(t_list *cmd, t_env **env)
{
	int		iterator;
	int		result;
	t_env	*new_elem;

	result = 0;
	iterator = 1;
	while (cmd->arguments[iterator])
	{
		if (ft_isalpha(cmd->arguments[iterator][0])	|| cmd->arguments[iterator][0] == '_')// аргум начинается с буквы или _
		{
			if (!find_argument_in_env(cmd->arguments[iterator], env))
			{//если аргумента нет в env
				new_elem = env_create_elem(cmd->arguments[iterator]);// может вернуть NULL
				if (!new_elem)//если память не выделена добавить обработку ошибки
					printf("new_elem == NULL, return ERROR\n");//perror()?
				env_lstadd_back(env, new_elem);
			}
			// {
			// 	env_lstadd_back(env, export_create_elem(cmd->arguments[iterator]));//или без проверки на NULL
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
		result = add_arguments(cmd, &env);// done
	else
		print_export(env);//про сортировку в чек листе не сказано 
	return (result);
}
