/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 19:02:19 by dirony            #+#    #+#             */
/*   Updated: 2022/05/09 13:48:36 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char 	*str;
	t_info	info;
	int		one_time_launch; //признак, что в аргументы передали -с и запускать надо один раз

	rl_outstream = stderr;
	info = (t_info){};//зануляет структуру - подсказка от Николая
	str = NULL;
	one_time_launch = 1;
	if (argc >= 3) //если при запуске в аргументах что-то есть - берем команду из них. Для тестов и для чеклиста
	{
		if (ft_strncmp(argv[1], "-c", 3) == 0)// проверить это условие в сабже
			str = argv[2];
	}
	using_history();/* initialize history */
	info.env = create_env(envp);
	while (one_time_launch)// && !is_exit_command(str))
	{
		info.envp = return_env_to_char(info.env);// заполняю массив строк значениями из связного списка // может вернуть NULL
		if (!str) //для разделения   запуске
		{
			ft_signal(1);
			str = readline(SHELL);// str нужно фришить (17:58)
			if (str)
				add_history(str);
			else// если передан ctrl D
			{
				free(str);
				clear_tokens(&info);
				ft_putstr_fd("\x1b[1F", 1);
				ft_putstr_fd(SHELL, 1);
				ft_putendl_fd("exit", 1);
				break ;
			}
		}
		else
			one_time_launch = 0;
		get_tokens_from_string(str, &info);//лексер
	
		put_tree_level_marks(&info); //пока здесь ставлю вызов, можно делать изнутри лексера
		put_group_id_marks(&info); //пока здесь ставлю вызов, можно делать изнутри лексера
		put_tree_marks(&info);
					//print_tokens(&info);
		// dollar_processing(&info);//WIP
		parse_and_execute_tree(&info);
		if (1)//!check_bad_syntax(&info))//если синтаксис хороший; проследить какой type используется для команд (сейчас всегда CMD)
		{
			// get_info_from_string(str, &info);//парсер
			// parse_commands(str, &info, info.envp);// пока выключу, буду тестировать новый запуск
				// print_commands_list(commands);
			if (is_exit_command(str))
			{
				free(str);
				break ;
			}
			// else
			// 	info.status = execute_commands(info.commands, info.envp, &info.env);
		}
		// printf_char_pointer(envp);
		// printf_env(env);
		clear_tokens(&info);
		free_string_array(info.envp);
		free(str);
		str = NULL;
	}
	rl_clear_history();
	clear_info(info);//зачаток общей функции, которая чистит всё
	return (info.status);
}

// сега при вызове ctrl D после какой нибудь команды
// сега если передать pwd |||||