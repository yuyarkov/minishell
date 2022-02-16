/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 19:02:19 by dirony            #+#    #+#             */
/*   Updated: 2022/02/16 21:33:38 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handler(int num)
{
	printf("got the signal code: %d\n", num);
}


int	parse_cmd(char *str, char **envp)
{
	char	**commands;
	int		num_of_commands;
			
	if (ft_strchr(str, '|'))
		commands = ft_split(str, '|');
	else
		commands = &str;
	num_of_commands = 0;
	while (commands[num_of_commands])
		num_of_commands++;
	add_cmd_to_list(num_of_commands, commands, envp);

	return (0);
}


int	main(int argc, char **argv, char **envp)
{
	(void) argc;
	(void) argv;
	char *str;
	
	signal(SIGINT, handler);//вот тут-то я ловлю сигнал ctrl+C, ctrl+D и ctrl+/
	using_history();    /* initialize history */
	str = NULL;
	while (ft_strncmp(str, "exit\0", 5) != 0)
	{
		str = readline("minishell$");//readline сама выводит строку приглашения
		add_history(str);
		parse_cmd(str, envp);
	}
}