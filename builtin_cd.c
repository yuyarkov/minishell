/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/20 12:58:46 by jg                #+#    #+#             */
/*   Updated: 2022/05/24 21:36:51 by fdarkhaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	search_oldpwd(t_env *env)
{
	while (env)
	{
		if (!ft_strncmp((env)->key, "OLDPWD", 6))
			return (1);
		env = (env)->next;
	}
	return (0);
}

char	*search_home(char **envp)
{
	int	iterator;

	iterator = 0;
	while (envp[iterator])
	{
		if (!ft_memcmp(envp[iterator], "HOME=", 5))
			return (envp[iterator] + 5);
		iterator++;
	}
	ft_putstr_fd("minishell: cd: HOME not set\n", 2);
	return (NULL);
}

void	make_oldpwd(char *oldpwd, t_env **env)
{
	char	*first_oldpwd;
	t_env	*new_elem;

	first_oldpwd = ft_strjoin("OLDPWD=", oldpwd);
	if (!first_oldpwd)
		exit(1);
	new_elem = env_create_elem(first_oldpwd);
	if (!new_elem)
		exit(1);
	env_lstadd_back(env, new_elem);
	free(first_oldpwd);
}

void	change_pwd(t_env **env, char *oldpwd)
{
	t_env	*tmp;
	char	pwd[1024];

	tmp = *env;
	while (tmp)
	{
		if (ft_strncmp(tmp->key, "PWD", ft_strlen(tmp->key)) == 0)
		{
			free(tmp->value);
			getcwd(pwd, 1024);
			tmp->value = ft_substr(pwd, 0, ft_strlen(pwd));
			if (!tmp->value)
				exit(1);
		}
		if (ft_strncmp(tmp->key, "OLDPWD", ft_strlen(tmp->key)) == 0)
		{
			free(tmp->value);
			tmp->value = ft_substr(oldpwd, 0, ft_strlen(oldpwd));
			if (!tmp->value)
				exit(1);
		}
		tmp = tmp->next;
	}
	if (!search_oldpwd(*env))
		make_oldpwd(oldpwd, env);
}

int	execute_cd_command(t_list *cmd, char **envp, t_env *env)
{
	char	*path;
	char	oldpwd[1024];

	if (!cmd->arguments[1])
	{
		path = search_home(envp);
		if (!path)
			return (1);
	}
	else
		path = cmd->arguments[1];
	getcwd(oldpwd, 1024);
	if (chdir(path) == 0)
		change_pwd(&env, oldpwd);
	else
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (1);
	}
	return (0);
}
