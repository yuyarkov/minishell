/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 19:02:19 by dirony            #+#    #+#             */
/*   Updated: 2022/04/12 21:30:04 by fdarkhaw         ###   ########.fr       */
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
		if (ft_strncmp(argv[1], "-c", 3) == 0)
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
			str = readline("minishell$ ");
			// ft_signal(2);
			add_history(str);
		}
		else
			one_time_launch = 0;
		if (get_info_from_string(str, &info))// если 1, то выхожу из цикла. это будет лексер
		{
			free_string_array(envp);
			break ;
		}
		commands = parse_commands(str, &info, envp);
			 //print_commands_list(commands);
		if (!is_exit_command(str))
		{
			status = execute_commands(commands, envp, &env);
			str = NULL;
		}
		// printf_char_pointer(envp);
		// printf_env(env);
		free_string_array(envp);
	}
	rl_clear_history();
	lstiter_env(env, free);//освобождаю поля в связном списке
	clear_list_env(env);
	//clear_env(env);//освобождаю память связного списка - эти функции можно объединить
	clear_info(&info, commands);
	return (status);
}
