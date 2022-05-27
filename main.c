/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 19:02:19 by dirony            #+#    #+#             */
/*   Updated: 2022/05/27 21:44:21 by fdarkhaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_numeric(char *s)
{
	int	i;
	int	result;

	if (!s)
		return (1);
	result = 1;
	i = 0;
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			result = 0;
		i++;
	}
	return (result);
}

int	is_exit_command(char *str, t_info *info)
{
	char	**split_arg;

	if (!str)
		return (0);
	split_arg = ft_split(str, ' ');
	if_pointer_is_null(split_arg);
	if (ft_strcmp(split_arg[0], "exit\0") == 0)// && split_arg[1])
	{
		if (!is_numeric(split_arg[1]))
		{
			ft_putstr_fd("exit\n", 1);
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(split_arg[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			info->status = 255;
			return (1);
		}
		else if (split_arg[2])
		{
			ft_putstr_fd("minishell: exit: too many arguments\n", 2);
			info->status = 1;
			printf("inside exit, status: %d\n", info->status);
			return (0);
		}
		else if (is_numeric(split_arg[1]) && !split_arg[2])
		{
			ft_putstr_fd("exit\n", 1);
			info->status = ft_atoi(split_arg[1]);
			return (1);
		}
	}
	free_string_array(split_arg);
	return (0);
}

int	free_after_ctrl_d(t_info *info, char **str)
{
	if (str)
		free(*str);
	clear_info(info);
	ft_putstr_fd("\x1b[1F", 1);
	ft_putstr_fd(SHELL, 1);
	ft_putendl_fd("exit", 1);
	//clear_info_except_envp(info);
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
		// ft_signal(1);
		info.envp = return_env_to_char(info.env);
		str = readline(SHELL);
		// printf("after rl - %s\n", str);
		if (is_exit_command(str, &info) == 1)
			break ;
	// printf("main - status: %d\n", info.status);
		if (str)
			add_history(str);
		else if (free_after_ctrl_d(&info, &str))
			break ;
		get_tokens_from_string(str, &info);//лексер и парсер
		// print_tokens(&info);
		clear_info_except_envp(&info);
		free(str);
		str = NULL;
	// printf("main - status: %d\n", info.status);
	}
	rl_clear_history();
	if (info.envp && info.changed_envp)
		free_string_array(info.envp);
	return (info.status);
}
