/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jg <jg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 13:44:38 by dirony            #+#    #+#             */
/*   Updated: 2022/03/09 22:38:58 by jg               ###   ########.fr       */
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
	}
	// ft_putstr_fd("minishell: cd: HOME not set\n", 2);
	return (NULL);//если нет HOME bash возвращает "bash: cd: HOME not set"; додумать после написания unset
}

int	execute_cd_command(t_list *cmd, char **envp)
{
	char	*path;

	if (!cmd->arguments[1])//если после cd нет ничего
		path = search_home(envp);// нужно взять путь из envp HOME= , и применить его как абсолютный
	else
		path = cmd->arguments[1];// теперь не надо освобождать память
	// path = ft_substr(cmd->cmd, 3, ft_strlen(cmd->cmd) - 3);
	printf("inside execute_cd, path: %s\n", path);
	if (chdir(path) != 0)//если не удалось перейти выводи ошибку
		perror("cd");
	//free(path);// освободить память
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
	char	**argv;
	int		argc;

	(void) envp;
	argv = ft_split(cmd->cmd, ' ');
	argc = 0;
	while (argv[argc])
		argc++;
	ft_putstr_fd("exit\n", 1);
	if (argc > 2)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2); //проверить fd для вывода ошибки
		return (1); //в этом случае из минишелла не выходим
	}
	if (argc > 0)
	{
		if (is_numeric(argv[1]))
		{
			exit_code = ft_atoi(argv[1]);
			exit(exit_code);
		}
		else
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(argv[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
		}
	}
	//ft_free(argv);// освободить память
	return (1);//какой должен быть код?
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
	//printf("==inside execute_echo\n");//принтф странно работает без \n
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
	//free(buf);// ???освободить память или нет???
	return (0);
}
