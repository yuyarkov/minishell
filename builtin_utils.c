/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jg <jg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 13:44:38 by dirony            #+#    #+#             */
/*   Updated: 2022/03/19 10:28:34 by jg               ###   ########.fr       */
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

int	execute_cd_command(t_list *cmd, char **envp, t_env *env)
{
	char	*path;

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
		// OLDPWD=PWD
		// PWD=path
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

int	is_numeric(char *s)
{
	int	i;
	int	result;

	result = 1;
	i = 0;
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			result = 0;
		i++;
	}
	return (result);
}

int	execute_exit_command(t_list *cmd, char **envp)
{
	int		exit_code;

	(void) envp;
	ft_putstr_fd("exit\n", 1);
	if (!is_numeric(cmd->arguments[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(cmd->arguments[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		exit(255);//bash закрывается с кодом 255
	}
	else if (cmd->arguments[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2); //Юра - проверить fd для вывода ошибки
		return (1); //в этом случае из минишелла не выходим
	}
	else if (is_numeric(cmd->arguments[1]) && !cmd->arguments[2])
	{
		exit_code = ft_atoi(cmd->arguments[1]);
		exit(exit_code);
	}
	return (0);// до этого момента не доходит, т.к. при exit без аргументов останавливается цикле в main
//Юра - какой должен быть код?
//Артём - если значение exit_code не указано, статусом выхода будет статус выхода последней выполненной команды
//сейчас так и работает
}

int	execute_echo_command(t_list *cmd, char **envp)
{
	int		iterator;

	(void) envp;
	iterator = 1;//Артём - при 1 флаг -n печатается))
				//Юра - почему 2? заменил на 1, заработало
	if (ft_strncmp(cmd->arguments[1], "-n\0", 3) == 0)//если есть -n iterator = 2
		iterator = 2;
	while (cmd->arguments[iterator])
	{
		ft_putstr_fd(cmd->arguments[iterator++], 1);
		if (cmd->arguments[iterator])
			ft_putchar_fd(' ', 1);
	}
	if (ft_strncmp(cmd->arguments[1], "-n\0", 3) != 0)//если нет -n пиши перевод каретки
		ft_putchar_fd('\n', 1);
	//printf("==inside execute_echo\n");
	return (0);//добавить обработку ошибок и обработать флаг -nnnnnnnn (должен работать как -n)
}

int	execute_pwd_command(t_list *cmd, char **envp)
{
	char	buf[1024];

	(void) envp;
	//printf("inside execute_pwd\n");//
	if (ft_strncmp(cmd->arguments[0], "pwd\0", 4) == 0)
	{
		getcwd(buf, 1024);
		ft_putstr_fd(buf, 1);
		ft_putchar_fd('\n', 1);
	}
	else
	{
		ft_putstr_fd(cmd->arguments[0], 2);
		ft_putstr_fd(": command not found\n", 2);
	}
	return (0);
}
