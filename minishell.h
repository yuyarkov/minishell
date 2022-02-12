/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 19:08:27 by dirony            #+#    #+#             */
/*   Updated: 2022/02/12 17:35:31 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <histedit.h>
# include <string.h>

typedef struct s_cmd
{
	int				flag_exit;
	int				single_quot;
	int				double_quot;//для парсинга кавычек, флаги загораются при открытии кавычки и гаснут при закрытии
	int				pipe;
	int				redirect;
	int				exit_code; //с каким кодом завершилась команда
	struct s_cmd	*next;
	struct s_cmd	*previous;//сделаем двусвязный, как в пайпексе
	
	
}	t_cmd;

void	ft_putstr_fd(char *s, int fd);


#endif