/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 19:02:19 by dirony            #+#    #+#             */
/*   Updated: 2022/02/15 21:24:41 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handler(int num)
{
	printf("got the signal code: %d\n", num);
}

int	main(int argc, char **argv)
{
	(void) argc;
	(void) argv;
	char *temp;
	
	signal(SIGINT, handler);//вот тут-то я ловлю сигнал ctrl+C, ctrl+D и ctrl+/
	using_history();    /* initialize history */
	temp = NULL;
	while (ft_strncmp(temp, "exit\0", 5) != 0)
	{
		temp = readline("minishell$");//readline сама выводит строку приглашения
		add_history(temp);
		parse_cmd(temp);
		printf("%s\n", temp);
	}
}