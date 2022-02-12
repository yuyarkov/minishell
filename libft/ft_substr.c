/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/09 18:01:40 by dirony            #+#    #+#             */
/*   Updated: 2021/10/16 14:32:38 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*result;
	size_t	i;
	size_t	size;

	if (!s)
		return (NULL);
	if (ft_strlen(s) <= start)
		return (ft_calloc(1, 1));
	size = ft_strlen(s) - start;
	if (size > len)
		size = len;
	result = malloc(sizeof(char) * (size + 1));
	if (NULL == result)
		return (NULL);
	i = 0;
	while (i < size)
	{
		result[i] = s[i + start];
		i++;
	}
	result[size] = '\0';
	return (result);
}
