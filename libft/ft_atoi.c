/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 23:08:25 by dirony            #+#    #+#             */
/*   Updated: 2021/10/16 17:20:15 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	check_structure(const char *str)
{
	int	i;
	int	count_digits;

	i = 0;
	count_digits = 0;
	while ((str[i] == ' ') || (str[i] == '\t') || (str[i] == '\n')
		|| (str[i] == '\v') || (str[i] == '\f') || (str[i] == '\r'))
		i++;
	if ((str[i] == '-') || (str[i] == '+'))
		i++;
	while (ft_isdigit(str[i]))
	{
		count_digits++;
		i++;
	}
	if (!count_digits)
		return (-2);
	return (i - count_digits);
}

int	ft_atoi(const char *str)
{
	int			i;
	int			minus;
	long long	result;

	minus = 1;
	result = 0;
	i = check_structure(str);
	if (i == -2)
		return (0);
	if (i > 0)
		if (str[i - 1] == '-')
			minus = -1;
	while (ft_isdigit(str[i]))
	{
		if (result >= 922337203685477580 && str[i] >= 7 && minus == -1)
			return (0);
		if (result >= 922337203685477580 && str[i] >= 7 && minus == 1)
			return (-1);
		result = result * 10;
		result = result + str[i] - 48;
		i++;
	}
	return ((int)result * minus);
}
