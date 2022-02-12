/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 21:49:24 by dirony            #+#    #+#             */
/*   Updated: 2021/10/16 17:28:31 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_start(char const *str, char const *set)
{
	int	i;

	i = 0;
	while (ft_strchr(set, str[i]) && str[i])
		i++;
	return (i);
}

static int	get_end(char const *str, char const *set)
{
	int	i;

	i = ft_strlen((char *)str);
	while (i > 0 && ft_strchr(set, str[i]))
		i--;
	return (i + 1);
}

char	*ft_strtrim(char const *str, char const *set)
{
	unsigned int		start;
	unsigned int		end;
	char				*result;

	if (!str)
		return (NULL);
	end = get_end(str, set);
	if (end == 0)
		return (ft_calloc(1, 1));
	start = get_start(str, set);
	if (ft_strlen(str) == 0 || end < start)
		result = ft_calloc(1, 1);
	else
		result = ft_substr(str, start, end - start);
	return (result);
}
