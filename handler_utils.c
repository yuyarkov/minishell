/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/09 17:09:26 by dirony            #+#    #+#             */
/*   Updated: 2022/04/12 21:50:41 by fdarkhaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handler(int num)
{
	// struct termios	silence;
	
	(void)num;
	// tcgetattr(STDIN_FILENO, &silence);
	// silence.c_lflag &= ~ECHOCTL;
	// tcsetattr(STDIN_FILENO, TCSANOW, &silence);
	ft_putstr_fd("\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handler_child(int num)
{
	ft_putstr_fd("HERE\n", 1);
	if (num == 3)
	{
		ft_putstr_fd("Quit :3\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	ft_signal(int i)
{
	if (i ==1)
	{
		rl_catch_signals = 0;//не отображает ^C и ^back_slash
		signal(SIGINT, &handler);//вот тут я ловлю сигнал ctrl+C
		signal(SIGQUIT, SIG_IGN);/* вот тут я ловлю сигнал ctrl+\ */	
	}
	else if (i == 2)//выполнение действий по умолчанию в дочернем процессе
	{
		// rl_catch_signals = 1;
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, SIG_DFL);
	}
	// else if (i == 2)//выполнение действий по умолчанию в дочернем процессе
	// {
	// 	// rl_catch_signals = 1;//отображает ^C и ^back_slash
	// 	signal(SIGINT, &handler_child);//вот тут я ловлю сигнал ctrl+C
	// 	signal(SIGQUIT, &handler_child);/* вот тут я ловлю сигнал ctrl+\ */
	// }
}
