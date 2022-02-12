/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 22:39:16 by dirony            #+#    #+#             */
/*   Updated: 2021/10/14 23:02:01 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_digits_num(unsigned int n)
{
	int	i;

	i = 0;
	if (n == 0)
		return (1);
	while (n)
	{
		n = n / 10;
		i++;
	}
	return (i);
}

static void	put_digits(char *result, unsigned int n, int i, int minus)
{
	while (i > minus)
	{
		i--;
		result[i] = n % 10 + '0';
		n = n / 10;
	}
	if (minus)
		result[0] = '-';
}

char	*ft_itoa(int num)
{
	unsigned int	n;
	int				i;
	int				minus;
	char			*result;

	minus = 0;
	if (num < 0)
	{
		minus = 1;
		n = -num;
	}
	else
		n = num;
	i = get_digits_num(n) + minus;
	result = malloc(i + 1);
	if (NULL == result)
		return (NULL);
	result[i] = '\0';
	put_digits(result, n, i, minus);
	return (result);
}
