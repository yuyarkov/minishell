/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 19:02:19 by dirony            #+#    #+#             */
/*   Updated: 2022/05/16 23:07:15 by fdarkhaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*str;
	t_info	info;
	int		one_time_launch; //признак, что в аргументы передали -с и запускать надо один раз

	rl_outstream = stderr;
	info = (t_info){};
	str = NULL;
	one_time_launch = 1;
	if (argc >= 3) //если при запуске в аргументах что-то есть - берем команду из них. Для тестов и для чеклиста
	{
		if (ft_strncmp(argv[1], "-c", 3) == 0)// проверить это условие в сабже
			str = argv[2];
	}
	using_history();
	info.env = create_env(envp);
	while (one_time_launch)
	{
		info.envp = return_env_to_char(info.env);// может вернуть NULL
		if (!str) //для разделения запуске
		{
			ft_signal(1);
			str = readline(SHELL);// str нужно фришить (17:58)
			if (str)
				add_history(str);
			else// если передан ctrl D
			{
				ft_ctrl_d(str, &info);
				break ;
			}
		}
		else
			one_time_launch = 0;
		get_tokens_from_string(str, &info);//лексер
		put_tree_level_marks(&info); //пока здесь ставлю вызов, можно делать изнутри лексера
		put_group_id_marks(&info); //пока здесь ставлю вызов, можно делать изнутри лексера
		put_tree_marks(&info);
					// print_tokens(&info);
		if (!check_bad_syntax(&info))//если синтаксис хороший; проследить какой type используется для команд (сейчас всегда CMD)
		{
			parse_and_execute_tree(&info);
			if (is_exit_command(str))
			{
				free(str);
				clear_tokens(&info);
				free_string_array(info.envp);
				clear_info(info);
				break ;
			}
		}
		clear_tokens(&info);
		free_string_array(info.envp);//в цикле не хватает очистки для cmd,сейчас утечка
		free(str);
		str = NULL;
	}
	rl_clear_history();
	clear_info(info);//зачаток общей функции, которая чистит всё
	return (info.status);
}

// сега при вызове ctrl D после какой нибудь команды
