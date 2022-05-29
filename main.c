/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 19:02:19 by dirony            #+#    #+#             */
/*   Updated: 2022/05/29 13:16:29 by dirony           ###   ########.fr       */
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
		if (str && *str != '\0')
			add_history(str);
		else if (!str && free_after_ctrl_d(&info, &str))
			break ;
		parse_and_execute_string(str, &info);
		clear_info_except_envp(&info);
		free(str);
		str = NULL;
	}
	rl_clear_history();
	return (info.status);
}
