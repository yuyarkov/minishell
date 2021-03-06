/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 18:30:08 by dirony            #+#    #+#             */
/*   Updated: 2021/10/16 17:22:23 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isalpha(int ch)
{
	if ((ch >= 'A') && (ch <= 'Z'))
		return (1);
	if ((ch >= 'a') && (ch <= 'z'))
		return (1);
	return (0);
}
