/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 19:02:19 by dirony            #+#    #+#             */
/*   Updated: 2022/05/25 21:37:20 by fdarkhaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_argc_argv(int ac, char **av)
{
	(void)ac;
	(void)av;
}

int	free_after_ctrl_d(char *str, t_info *info)
{
	free(str);
	clear_info(info);
	ft_putstr_fd("\x1b[1F", 1);
	ft_putstr_fd(SHELL, 1);
	ft_putendl_fd("exit", 1);
	//clear_info_except_envp(info);
	// free(str);//зачем фришить str дважды?
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	char	*str;
	t_info	info;

	ft_argc_argv(argc, argv);
	// rl_outstream = stderr;//что делает эта строка
	info = (t_info){};
	str = NULL;
	using_history();
	info.env = create_env(envp);
	while (1)
	{
		info.envp = return_env_to_char(info.env);
		if (!str)//подумать, как собрать в ft_readline
		{
			ft_signal(1);
			str = readline(SHELL);
			if (str)
				add_history(str);
			else if (free_after_ctrl_d(str, &info))// если передан ctrl D
				break ;
		}
		get_tokens_from_string(str, &info);//лексер
					//print_tokens(&info);
		if (!check_bad_syntax(&info))//если синтаксис хороший; проследить какой type используется для команд (сейчас всегда CMD)
		{
			parse_and_execute_tree(&info);
			if (is_exit_command(str, &info))
				break ;
		}
		clear_info_except_envp(&info);
		free(str);
		str = NULL;
	}
	if (str)
		free(str);
	rl_clear_history();
	if (info.envp && info.changed_envp)
		free_string_array(info.envp);
	return (info.status);
}
