/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 19:02:19 by dirony            #+#    #+#             */
/*   Updated: 2022/05/24 20:57:13 by dirony           ###   ########.fr       */
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
		info.envp = return_env_to_char(info.env);
		if (!str)
		{
			ft_signal(1);
			str = readline(SHELL);// str нужно фришить (17:58)
			if (str)
				add_history(str);
			else// если передан ctrl D
			{
				free_after_ctrl_d(str, &info);
				break ;
			}
		}
		else
			one_time_launch = 0;
		get_tokens_from_string(str, &info);//лексер
					print_tokens(&info);
		if (!check_bad_syntax(&info))//если синтаксис хороший; проследить какой type используется для команд (сейчас всегда CMD)
		{
			parse_and_execute_tree(&info);
			if (is_exit_command(str))
			{
				free(str);
				clear_info(&info);
				break ;
			}
		}
		clear_info_except_envp(&info);
		free(str);
		str = NULL;
	}
	rl_clear_history();
	if (info.envp && info.changed_envp)
		free_string_array(info.envp);
	return (info.status);
}

// сега при вызове ctrl D после какой нибудь команды
