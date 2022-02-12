/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 20:22:22 by dirony            #+#    #+#             */
/*   Updated: 2021/10/14 20:23:32 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *str)
{
	char	*result;

	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	ft_strlcpy(result, str, ft_strlen(str) + 1);
	return (result);
}
