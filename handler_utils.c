/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/09 17:09:26 by dirony            #+#    #+#             */
/*   Updated: 2022/04/11 19:56:07 by fdarkhaw         ###   ########.fr       */
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

void	ft_signal(void)
{
	rl_catch_signals = 0;
	signal(SIGINT, &handler);//вот тут я ловлю сигнал ctrl+C
	signal(SIGQUIT, SIG_IGN);/* вот тут я ловлю сигнал ctrl+\ */
}
