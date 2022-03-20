/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jg <jg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/20 12:58:46 by jg                #+#    #+#             */
/*   Updated: 2022/03/20 15:03:43 by jg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*search_home(char **envp)
{
	int	iterator;

	iterator = 0;
	while (envp[iterator])
	{
		if (!ft_memcmp(envp[iterator], "HOME=", 5))
			return (envp[iterator] + 5);
		iterator++;
	}//если нет HOME bash возвращает "bash: cd: HOME not set" и 1
	ft_putstr_fd("minishell: cd: HOME not set\n", 2);
	return (NULL);
}

char	*search_oldpwd(char **envp)
{
	int	iterator;

	iterator = 0;
	while (envp[iterator])
	{
		if (!ft_strncmp(envp[iterator], "PWD=", 4))
			return (envp[iterator] + 4);
		iterator++;
	}
	return ("\0");//если нет PWD
}

void	change_pwd(t_env **env, char *oldpwd)
{
	t_env	*tmp;
	char	pwd[1024];

	tmp = *env;
	getcwd(pwd, 1024);
	while (tmp)
	{
		if (ft_strncmp(tmp->key, "PWD", ft_strlen(tmp->key)) == 0)
		{
			free(tmp->value);
			tmp->value = ft_substr(pwd, 0, ft_strlen(pwd));
		}
		if (ft_strncmp(tmp->key, "OLDPWD", ft_strlen(tmp->key)) == 0)
		{
			free(tmp->value);
			tmp->value = ft_substr(oldpwd, 0, ft_strlen(oldpwd));
		}
		tmp = tmp->next;
	}
	*env = tmp;
}

int	execute_cd_command(t_list *cmd, char **envp, t_env *env)
{
	char	*path;
	char	*oldpwd;

	(void)env;
	if (!cmd->arguments[1])//если после cd нет ничего
	{
		path = search_home(envp);// нужно взять путь из envp HOME
		if (!path)//если нет HOME bash возвращает "bash: cd: HOME not set" и 1
			return (1);
	}
	else
		path = cmd->arguments[1];
	if (chdir(path) == 0)// при каждом корректном вызове cd
	{
		oldpwd = search_oldpwd(envp);
		change_pwd(&env, oldpwd);
	}
	else//иначе - выводи ошибку
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (1);
	}
	return (0);
}
