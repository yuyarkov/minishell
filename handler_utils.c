/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/09 17:09:26 by dirony            #+#    #+#             */
/*   Updated: 2022/04/16 21:25:35 by fdarkhaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handler(int num)
{
	(void)num;
	ft_putstr_fd("\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	ft_signal(int i)
{
	if (i ==1)//родительский процесс
	{
		rl_catch_signals = 0;//не отображает ^C и ^back_slash
		signal(SIGINT, &handler);//вот тут я ловлю сигнал ctrl+C
		signal(SIGQUIT, SIG_IGN);/* вот тут я ловлю сигнал ctrl+\ */	
	}
	else if (i == 2)//дочерний процесс
	{
		signal(SIGQUIT, SIG_DFL);//выполнение действий по умолчанию
		signal(SIGINT, SIG_DFL);
	}
}
