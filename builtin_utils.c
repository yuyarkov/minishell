/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 13:44:38 by dirony            #+#    #+#             */
/*   Updated: 2022/02/26 17:54:56 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_cd_command(t_list *cmd, char **envp)
{
	char	*path;
	

	(void) envp;
	path = ft_substr(cmd->cmd, 3, ft_strlen(cmd->cmd) - 3);
	//printf("inside execute_cd, path: %s\n", path);
	chdir(path); //заработало. добавить обработку ошибок
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
	char 	**argv;
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
	return (1);//какой должен быть код?
}