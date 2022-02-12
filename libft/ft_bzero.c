/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 18:58:38 by dirony            #+#    #+#             */
/*   Updated: 2021/10/16 17:21:58 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t num_bytes)
{
	unsigned char	*str;
	size_t			i;

	if (num_bytes > 0)
	{
		str = (unsigned char *)s;
		i = 0;
		while (i < num_bytes)
		{
			str[i] = '\0';
			i++;
		}
	}
}
