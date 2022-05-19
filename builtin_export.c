/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 15:53:52 by fdarkhaw          #+#    #+#             */
/*   Updated: 2022/05/19 23:31:30 by fdarkhaw         ###   ########.fr       */
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
	if (ft_strchr(str, '='))//если есть =
		if_there_is_equal_sign(str, &new_elem);
	else//если нет =
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
