/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 19:02:19 by dirony            #+#    #+#             */
/*   Updated: 2022/04/16 17:02:54 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char 	*str;
	t_list	*commands;
	int		status;
	t_info	info;
	t_env	*env;
	int		one_time_launch; //признак, что в аргументы передали -с и запускать надо один раз

	rl_outstream = stderr;//что это?
	str = NULL;
	one_time_launch = 1;
	status = 0; //возвращать статус из дочерних процессов
	if (argc >= 3) //если при запуске в аргументах что-то есть - берем команду из них. Для тестов и для чеклиста
	{
		if (ft_strncmp(argv[1], "-c", 3) == 0)// проверить это условие в сабже
			str = argv[2];
	}
	using_history();    /* initialize history */
	env = create_env(envp);
	while (one_time_launch && !is_exit_command(str))
	{
		envp = return_env_to_char(env);// заполняю массив строк значениями из связного списка // может вернуть NULL
		if (!str) //для разделения   запуске
		{
			ft_signal(1);
			str = readline("minishell$ ");// str нужно фришить (17:58)
			// ft_signal(2);
			if (str)
				add_history(str);
			else// если передан ctrl D
			{
				free_string_array(envp);
				ft_putendl_fd("exit", 1);
				break ;
			}
		}
		else
			one_time_launch = 0;
		get_tokens_from_string(str, &info);
		get_info_from_string(str, &info);
		print_tokens(&info);
		commands = parse_commands(str, &info, envp);
			 //print_commands_list(commands);
		if (!is_exit_command(str))
		{
			status = execute_commands(commands, envp, &env);
			str = NULL;
		}
		// printf_char_pointer(envp);
		// printf_env(env);
		clear_tokens(&info);
		free_string_array(envp);
	}
	rl_clear_history();
	lstiter_env(env, free);//освобождаю поля в связном списке
	clear_list_env(env);
	//clear_env(env);//освобождаю память связного списка - эти функции можно объединить
	clear_info(&info, commands);
	return (status);
}
