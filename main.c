/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 19:02:19 by dirony            #+#    #+#             */
/*   Updated: 2022/02/23 16:03:16 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handler(int num)
{
	printf("got the signal code: %d\n", num);
}

void	print_commands_list(t_list *cmd)
{
	t_list	*iterator;

	iterator = cmd;
	while (iterator)
	{
		printf("print_commands_list. parsed command: %s\n", iterator->cmd);
		iterator = iterator->next;
	}	
}


t_list	*parse_commands(char *str, t_info *info, char **envp)
{
	char	**commands;
	t_list	*result;
	int		num_of_commands;
	
	(void) info;
	if (ft_strchr(str, '|')) // здесь надо будет выделить в отдельную функцию поиск и разделение по ;, && и т.д.
		commands = ft_split(str, '|');
	else
	{
		commands = malloc(sizeof(char *) * 2);
		commands[0] = str; //и помечаю пустым последний элемент массива строк
		commands[1] = NULL;
	}
	num_of_commands = 0;
	while (commands[num_of_commands])
		num_of_commands++;
	//printf("commands before add: %s, num_of_commands: %d\n", commands[0], num_of_commands);
	result = add_cmd_to_list(num_of_commands, commands, envp);

	return (result);
}

void	execute_cmd(t_list *cmd, char **envp)
{
	//printf("вот такую команду исполняю: %s\n", cmd->cmd);
	if (execve(cmd->cmd, cmd->arguments, envp) == -1)
		perror ("Could not execve");
	exit(EXIT_SUCCESS);//подумать, как брать корректный код выхода из execve
}

int	execute_builtin(t_list *cmd, char **envp)
{
	if (ft_strncmp(cmd->cmd, "cd", 2) == 0)
		return (execute_cd_command(cmd, envp));
	if (ft_strncmp(cmd->cmd, "exit ", 5) == 0)
		return (execute_exit_command(cmd, envp));
	return (0);
}

int	execute_commands(t_list *commands, char **envp)
{
	pid_t	child;
	int		status;
	t_list	*iter;
	
	status = 0;
	iter = commands;
	while(iter)
	{
		if (is_builtin_command(iter->cmd))
			status = execute_builtin(iter, envp);
		else
		{
			child = fork();
			if (child < 0)
			{
				perror("Fork: ");
				return (-1);//подумать, какой правильный код возвращать
			}
			if (child == 0)
				execute_cmd(iter, envp);
			waitpid(child, &status, 0);
		}
		iter = iter->next;
	}
	return (status);
}



int	main(int argc, char **argv, char **envp)
{
	rl_outstream = stderr;
	char 	*str;
	t_list	*commands;
	int		status;
	t_info	info;
	
	str = NULL;
	status = 0; //возвращать статус из дочерних процессов
	if (argc >= 3)
	{
		if (ft_strncmp(argv[1], "-c", 3) == 0)
			str = argv[2];
	}
						// if (execve("/bin/ls", argv, envp) == -1) 
						// 	perror ("Could not execve /bin/ls");
	signal(SIGINT, handler);//вот тут-то я ловлю сигнал ctrl+C, ctrl+D и ctrl+/
	using_history();    /* initialize history */
	while (!is_exit_command(str))
	{
		if (!str) //для разделения случая, когда команда пришла в аргументах при запуске
		{
			str = readline("minishell$");//readline сама выводит строку приглашения
			add_history(str);
		}
		get_info_from_string(str, &info);
						// printf("info->num_of_commands: %d\n", info.num_of_commands);
						// for (int i = 0; i < info.num_of_commands - 1; i++)
						// 	printf("limeter[%d]: %d\n", i, info.limiters[i]);
		commands = parse_commands(str, &info, envp);
						//print_commands_list(commands);
		if (!is_exit_command(str))//ничего поизящнее не придумал
		{
			status = execute_commands(commands, envp);
			str = NULL;
		}
				//где-то здесь нужно освобождать структуры
	}
	
	
	return (status);
}
