/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 19:02:19 by dirony            #+#    #+#             */
/*   Updated: 2022/02/18 21:16:31 by dirony           ###   ########.fr       */
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
		printf("parsed command: %s\n", iterator->cmd);
		iterator = iterator->next;
	}	
}


t_list	*parse_cmd(char *str, char **envp)
{
	char	**commands;
	t_list	*result;
	int		num_of_commands;
			
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
	printf("commands before add: %s, num_of_commands: %d\n", commands[0], num_of_commands);
	result = add_cmd_to_list(num_of_commands, commands, envp);

	return (result);
}

void	execute_cmd(t_list *cmd, char **envp)
{
	if (execve(cmd->cmd, cmd->arguments, envp) == -1)
		perror ("Could not execve");
	exit(EXIT_FAILURE);
}

void	execute_commands(t_list *commands, char **envp)
{
	pid_t	child;
	int		status;
	t_list	*iter;
	
	iter = commands;
	while(iter)
	{
		child = fork();
		if (child < 0)
			return (perror("Fork: "));
		if (child == 0)
			execute_cmd(iter, envp);
		waitpid(child, &status, 0);
		iter = iter->next;
	}
}

int	main(int argc, char **argv, char **envp)
{

	char 	*str;
	t_list	*commands;
	(void) argc;
	(void) argv;
	
	signal(SIGINT, handler);//вот тут-то я ловлю сигнал ctrl+C, ctrl+D и ctrl+/
	using_history();    /* initialize history */
	str = NULL;
	while (ft_strncmp(str, "exit\0", 5) != 0)
	{
		str = readline("minishell$");//readline сама выводит строку приглашения
		add_history(str);
		commands = parse_cmd(str, envp);
		print_commands_list(commands);
		execute_commands(commands, envp);
	}
	
	return (0);
}
