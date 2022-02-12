/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/09 14:20:43 by dirony            #+#    #+#             */
/*   Updated: 2021/10/09 14:49:44 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int ch)
{
	char	*str;
	size_t	i;

	str = (char *)s;
	i = ft_strlen(str);
	if ((char)ch == '\0')
		return (&str[i]);
	while (i)
	{
		if (str[i - 1] == (char)ch)
			return (&str[i - 1]);
		i--;
	}
	return (NULL);
}
