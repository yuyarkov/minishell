/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/20 12:58:46 by jg                #+#    #+#             */
/*   Updated: 2022/03/29 20:36:41 by fdarkhaw         ###   ########.fr       */
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

int	search_oldpwd(t_env *env)
{
	while (env)
	{
		if (!ft_strncmp((env)->key, "OLDPWD", 6))
			return (1);//если есть OLDPWD
		env = (env)->next;
	}
	return (0);//если нет OLDPWD
}

char	*search_pwd(char **envp)
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
	t_env	*new_elem;
	char	*first_oldpwd;

	tmp = *env;
	while (tmp)
	{
		if (ft_strncmp(tmp->key, "PWD", ft_strlen(tmp->key)) == 0)
		{
			free(tmp->value);
			getcwd(pwd, 1024);
			tmp->value = ft_substr(pwd, 0, ft_strlen(pwd));// может вернуть NULL
		}
		if (ft_strncmp(tmp->key, "OLDPWD", ft_strlen(tmp->key)) == 0)
		{
			free(tmp->value);
			tmp->value = ft_substr(oldpwd, 0, ft_strlen(oldpwd));// может вернуть NULL
		}
		tmp = tmp->next;
	}
	if (!search_oldpwd(*env))// если нет олдпвд
	{
		first_oldpwd = ft_strjoin("OLDPWD=", oldpwd);// может вернуть NULL
		if (!first_oldpwd)//если память не выделена добавить обработку ошибки
			printf("first_oldpwd == NULL, return ERROR\n");//perror()?
		new_elem = env_create_elem(first_oldpwd);// может вернуть NULL
		if (!new_elem)//если память не выделена добавить обработку ошибки
			printf("new_elem == NULL, return ERROR\n");//perror()?
		env_lstadd_back(env, new_elem);
		free(first_oldpwd);
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
		oldpwd = search_pwd(envp);
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
