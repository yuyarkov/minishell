/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 22:33:18 by dirony            #+#    #+#             */
/*   Updated: 2022/05/29 12:30:42 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*result;
	size_t	i;
	size_t	j;

	if (!s2)
		return (ft_strdup(s1));
	result = malloc(ft_strlen((char *)s1) + ft_strlen((char *)s2) + 1);
	if (NULL == result)
		return (NULL);
	i = 0;
	j = 0;
	while (i < ft_strlen((char *)s1))
	{
		result[i] = s1[i];
		i++;
	}
	while (j < ft_strlen((char *)s2))
	{
		result[i] = s2[j];
		i++;
		j++;
	}
	result[i] = '\0';
	return (result);
}
