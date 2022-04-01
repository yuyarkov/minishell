/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 12:42:00 by fdarkhaw          #+#    #+#             */
/*   Updated: 2022/04/01 19:25:32 by fdarkhaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_max_strlen(char *str1, char *str2)
{
	int	s1;
	int	s2;

	s1 = ft_strlen(str1);
	s2 = ft_strlen(str2);
	if (s1 > s2)
		return (s1);
	else
		return (s2);
}
