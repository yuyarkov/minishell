/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 15:53:52 by fdarkhaw          #+#    #+#             */
/*   Updated: 2022/03/23 19:37:36 by fdarkhaw         ###   ########.fr       */
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

int	parser_arguments(t_list *cmd, t_env **env)
{
	int	iterator;
	int	result;
	// char	**split

	result = 0;
	iterator = 1;
	while (cmd->arguments[iterator])
	{
		// if ()
		if (ft_isalpha(cmd->arguments[iterator][0]) \
		|| cmd->arguments[iterator][0] == '_')//create_variable
			env_lstadd_back(env, env_create_elem(cmd->arguments[iterator]));
		else
		{
			result = 1;
			ft_putstr_fd( "minishell: export: `", 2);
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
		result = parser_arguments(cmd, &env);
	else
		sorting_and_print_env(env);
	return (result);
}
