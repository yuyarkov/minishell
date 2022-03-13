/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jg <jg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 19:02:19 by dirony            #+#    #+#             */
/*   Updated: 2022/03/13 20:39:45 by jg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handler(int num)
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

char	**split_commands_by_limiters(char *str, t_info *info)
{
	char	**result;
	char	*temp;
	int		length;
	int		index;
	int		i;

	result = malloc(sizeof(char *) * (info->num_of_commands + 1));
	if (NULL == result)
		exit(EXIT_FAILURE);
	result[0] = ft_strtrim(ft_substr(str, 0, info->limiters[0].index), SPACES);//подумать над освобождением строк после trim и substr
	//printf("result[0]: %s\n", result[0]);
	i = 1;
	while (i < info->num_of_commands)
	{
		index = info->limiters[i - 1].index + (info->limiters[i - 1].sign % 2 + 1);
		length = info->limiters[i].index - index;
		temp = ft_substr(str, index, length);
		result[i] = ft_strtrim(temp, SPACES);
		free(temp);
		//printf("result[%d]: %s\n", i, result[i]);
		i++;
	}
	result[i] = NULL;
	return (result);
}

t_list	*parse_commands(char *str, t_info *info, char **envp)
{
	char	**commands;
	t_list	*result;

	if (info->num_of_commands > 1) // здесь надо будет выделить в отдельную функцию поиск и разделение по ;, && и т.д.
		commands = split_commands_by_limiters(str, info);
	else
	{
		commands = malloc(sizeof(char *) * 2);
		commands[0] = str; //и помечаю пустым последний элемент массива строк
		commands[1] = NULL;
	}
	//printf("commands before add: %s, num_of_commands: %d\n", commands[0], info->num_of_commands);
	
	result = add_cmd_to_list(info->num_of_commands, commands, envp);
	//добавить освобождение commands и строк
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
	if (ft_strncmp(cmd->cmd, "echo ", 5) == 0)
		return (execute_echo_command(cmd, envp));
	if (ft_strncmp(cmd->cmd, "pwd", 3) == 0)
		return (execute_pwd_command(cmd, envp));
	return (0);
}

int	execute_commands(t_list *commands, char **envp)
{
	pid_t	child;
	int		status;
	t_list	*iter;

	status = 0;
	iter = commands;
	while (iter)
	{
		if (is_builtin_command(iter->cmd))
			status = execute_builtin(iter, envp);
		else if (*iter->cmd != '\0')
		{
			//printf("iter->cmd: %s\n", iter->cmd);
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
	while (!is_exit_command(str))//Артём - можно запустить бесконечный цикл, например while (true), и прописать последующие действия в ифах;
								//Юра - надо будет менять условие для бесконечного цикла
	{
		if (!str) //для разделения   запуске
		{
			str = readline("minishell$ ");//readline сама выводит строку приглашения
			add_history(str);
		}
		get_info_from_string(str, &info);
						// printf("info->num_of_commands: %d\n", info.num_of_commands);
						// for (int i = 0; i < info.num_of_commands - 1; i++)
						// 	printf("limeter[%d]: %d\n", i, info.limiters[i]);
		commands = parse_commands(str, &info, envp);// Артём - если перед командой, после команды и между командой и аргументом есть пробелы, парсер должен их отрезать (см. bash).
													//Сейчас ругается ошибкой "Could not execve: Permission denied" (пример "    cd    ..    ")
						// print_commands_list(commands);
		if (!is_exit_command(str))//ничего поизящнее не придумал
		{
			status = execute_commands(commands, envp);
			str = NULL;
		}
//где-то здесь нужно освобождать структуры
	}
	return (status);
}
