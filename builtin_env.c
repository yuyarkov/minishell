/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jg <jg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 21:39:09 by jg                #+#    #+#             */
/*   Updated: 2022/03/15 12:36:14 by jg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_env_command(t_list *cmd, char **envp)
{
	(void) cmd;
	while (*envp)
		ft_putendl_fd(*(envp++), 1);
	return (0);
}

char	**our_env(char **envp)
{
	int		size;
	int		index;
	char	**new_env;

	size = 0;
	while (envp[size])
		size++;
	new_env = (char **)malloc(sizeof(char *) * (size + 1));
	index = 0;
	new_env[index++] = ft_strdup("minishell=0");
	while (index < size - 1)
	{
		new_env[index] = ft_strdup(envp[index]);
		index++;
	}
	new_env[index] = NULL;
	return (new_env);
}

void	free_str_pointer(char **str)
{
	int	iterator;

	iterator = 0;
	while (str[iterator])
	{
		free(str[iterator]);
		iterator++;
	}
	free(str);
}

/*
int	execute_env_command(t_list *cmd, char **envp)
{
	int		iterator;

	(void) envp;
	iterator = 0;
	while (cmd->env[iterator])
	{
		ft_putstr_fd(cmd->env[iterator]->key, 1);
		ft_putchar_fd('=', 1);
		ft_putendl_fd(cmd->env[iterator]->value, 1);
		iterator++;
	}
	return (0);
}

int	create_env(t_env *env, char **envp)
{
	int		iterator;
	char	**result;

	iterator = 0;
	while (envp[iterator])
	{
		result = ft_split(envp[iterator], '=');
		env[iterator]->key = ft_substr(result[0], 0, ft_strlen(result[0]));
		env[iterator]->value = ft_substr(result[1], 0, ft_strlen(result[1]));
		free(result);
		iterator++;
	}
	env[iterator] = NULL;
	return (0);
}
*/