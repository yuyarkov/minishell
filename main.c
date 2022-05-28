/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 19:02:19 by dirony            #+#    #+#             */
/*   Updated: 2022/05/28 16:51:02 by fdarkhaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	free_after_ctrl_d(t_info *info, char **str)
{
	if (str)
		free(*str);
	clear_info(info);
	ft_putstr_fd("\x1b[1F", 1);
	ft_putstr_fd(SHELL, 1);
	ft_putendl_fd("exit", 1);
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	char	*str;
	t_info	info;

	rl_outstream = stderr;
	info = (t_info){};
	using_history();
	info.env = create_env(envp, argc, argv);
	while (1)
	{
		info.envp = return_env_to_char(info.env);
		str = readline(SHELL);
		if (is_exit_command(str, &info) == 1)
			break ;
		if (str)
			add_history(str);
		else if (free_after_ctrl_d(&info, &str))
			break ;
		get_tokens_from_string(str, &info);
		clear_info_except_envp(&info);
		free(str);
		str = NULL;
	}
	rl_clear_history();
	return (info.status);
}
