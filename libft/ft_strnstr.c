/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/09 16:33:42 by dirony            #+#    #+#             */
/*   Updated: 2021/10/09 17:53:48 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;
	char	*res;

	res = (char *)haystack;
	if (needle[0] == '\0')
		return (res);
	if (len == 0)
		return (NULL);
	i = 0;
	while ((i < len) && res[i])
	{
		j = 0;
		while ((res[i + j] == needle[j]) && res[i + j] && ((i + j) < len))
		{
			if (needle[j + 1] == '\0')
				return (&res[i]);
			j++;
		}
		i++;
	}
	return (NULL);
}
