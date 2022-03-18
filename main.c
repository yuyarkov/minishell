/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 19:02:19 by dirony            #+#    #+#             */
/*   Updated: 2022/03/18 20:57:13 by dirony           ###   ########.fr       */
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

int	execute_builtin(t_list *cmd, char **envp, t_env **env)
{
	if (ft_strncmp(cmd->cmd, "cd", 2) == 0)
		return (execute_cd_command(cmd, envp));
	if (ft_strncmp(cmd->cmd, "exit ", 5) == 0)
		return (execute_exit_command(cmd, envp));
	if (ft_strncmp(cmd->cmd, "echo ", 5) == 0)
		return (execute_echo_command(cmd, envp));
	if (ft_strncmp(cmd->cmd, "pwd", 3) == 0)
		return (execute_pwd_command(cmd, envp));
	if (ft_strncmp(cmd->cmd, "env\0", 4) == 0)
		return (execute_env_command(cmd, envp));
	if (ft_strncmp(cmd->cmd, "unset\0", 6) == 0)
		return (execute_unset_command(cmd, envp, *env));
	return (0);
}

int	execute_commands(t_list *commands, char **envp, t_env **env)
{
	pid_t	child;
	int		status;
	t_list	*iter;

	status = 0;
	iter = commands;

	while (iter)
	{
		if (is_builtin_command(iter->cmd))
			status = execute_builtin(iter, envp, env);
		else if (iter->cmd && *iter->cmd != '\0')
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

void	printf_env(t_env *env)
{
	while (env)
	{
		printf("k - %s === v - %s\n", env->key, env->value);
		env = env->next;
	}
}

void	printf_char_pointer(char **str)
{
	int	i = 0;
	while (str[i])
		printf("our envp = %s\n", str[i++]);
}

int	main(int argc, char **argv, char **envp)
{
	rl_outstream = stderr;
	char 	*str;
	t_list	*commands;
	int		status;
	t_info	info;
	t_env	*env;
	int		one_time_launch; //признак, что в аргументы передали -с и запускать надо один раз

	str = NULL;
	one_time_launch = 1;
	status = 0; //возвращать статус из дочерних процессов
	if (argc >= 3)
	{
		if (ft_strncmp(argv[1], "-c", 3) == 0)
			str = argv[2];
	}
	signal(SIGINT, handler);//вот тут-то я ловлю сигнал ctrl+C, ctrl+D и ctrl+/
	using_history();    /* initialize history */
	env = create_env(envp);//заполняю связный список значениями из envp
	// printf_env(env);
	while (one_time_launch && !is_exit_command(str))//Артём - не надо менять, потому что логика верная - после exit без аргумента статус выхода сохраняется от предыдущего процесса
								//Юра - надо будет менять условие для бесконечного цикла
	{
		envp = return_env_to_char(env);// заполняю массив строк значениями из связного списка
		// printf_char_pointer(envp);
		if (!str) //для разделения   запуске
		{
			str = readline("minishell$ ");//readline сама выводит строку приглашения
			add_history(str);
		}
		else
			one_time_launch = 0;
		get_info_from_string(str, &info);
		commands = parse_commands(str, &info, envp);// Артём - если перед командой, после команды и между командой и аргументом есть пробелы, парсер должен их отрезать (см. bash).
													//Сейчас ругается ошибкой "Could not execve: Permission denied" (пример "    cd    ..    ")
						// print_commands_list(commands);
		if (!is_exit_command(str))//ничего поизящнее не придумал
		{
			status = execute_commands(commands, envp, &env);
			str = NULL;
		}
//где-то здесь нужно освобождать структуры
		free_str_pointer(envp);// освобождаю массив строк
	}
	lstiter_env(env, free);//освобождаю поля в связном списке
	clear_env(env);//освобождаю память связного списка
	return (status);
}
