/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 15:53:52 by fdarkhaw          #+#    #+#             */
/*   Updated: 2022/05/21 21:49:31 by fdarkhaw         ###   ########.fr       */
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

	new_elem = malloc(sizeof(t_env));
	if_value_is_null(new_elem);
	new_elem->next = NULL;
	if (ft_strchr(str, '='))
		if_there_is_equal_sign(str, &new_elem);
	else
	{
		new_elem->key = ft_substr(str, 0, ft_strlen(str));
		if_value_is_null(new_elem->key);
		new_elem->value = NULL;
	}
	return (new_elem);
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
		if (ft_isalpha(cmd->arguments[iterator][0]) \
			|| cmd->arguments[iterator][0] == '_')
		{
			if (!find_argument_in_env(cmd->arguments[iterator], env))
				new_elem = if_arg_is_not_in_env(cmd->arguments[iterator], env);
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

int	find_key_in_list(char *argument, t_env *list)
{
	char	**sparg;
	int		a_in_l;

	sparg = ft_split(argument, '=');
	if_pointer_is_null(sparg);
	while (list)
	{
		a_in_l = ft_strncmp(sparg[0], list->key, max_strlen(sparg[0], list->key));
		if (!a_in_l)
		{
			free_string_array(sparg);
			return (1);
		}
		list = list->next;
	}
	free_string_array(sparg);
	return (0);
}

char	*find_next_min(char **envp, t_env *list)
{
	char	*str_min;
	int		i;

	i = 0;
	str_min = ft_substr("z", 0, 1);
	while (envp[i])
	{
		if (find_key_in_list(envp[i], list))
			i++;
		else if (ft_strcmp(str_min, envp[i]) > 0)
		{
			free(str_min);
			str_min = ft_substr(envp[i], 0, ft_strlen(envp[i]));
			i++;
		}
		else
			i++;
	}
	return (str_min);
}

t_env	*create_sort_env(char **envp)
{
	int		i;
	t_env	*new_elem;
	t_env	*list;
	char	*min_str;

	list = NULL;
	i = 0;
	while (envp[i])
	{
		min_str = find_next_min(envp, list);
		new_elem = env_create_elem(min_str);
		env_lstadd_back(&list, new_elem);
		free(min_str);
		i++;
	}
	return (list);
}

int	execute_export_command(t_list *cmd, char **envp, t_env *env)
{
	int		result;
	t_env	*tmp;

	result = 0;
	if (cmd->arguments[1])
		result = add_arguments(cmd, &env);
	else
	{
		tmp = create_sort_env(envp);
		print_export(tmp);
		lstiter_env(tmp, free);
	}
	return (result);
}
