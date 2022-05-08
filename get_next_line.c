/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 11:39:30 by dirony            #+#    #+#             */
/*   Updated: 2022/05/08 15:42:47 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_line	*create_tail(int fd)
{
	t_line	*result;

	if (fd < 0)
		return (NULL);
	result = malloc(sizeof(t_line));
	if (NULL == result)
		return (NULL);
	result->buff = malloc(BUFFER_SIZE + 1);
	if (NULL == result->buff)
	{
		free(result);
		return (NULL);
	}
	result->buff_size = BUFFER_SIZE + 1;
	result->string = NULL;
	result->fd = fd;
	result->bytes_read = 0;
	result->has_newline = 0;
	result->last_buff = 0;
	result->last_line = 0;
	return (result);
}

char	*get_line_from_string(t_line *tail)
{
	char	*p_newline;
	char	*result;
	char	*temp;

	if (tail->last_line)
		return (NULL);
	p_newline = ft_strchr(tail->string, '\n');
	if (p_newline)
	{
		result = malloc(p_newline - tail->string + 2);
		ft_strlcpy(result, tail->string, p_newline - tail->string + 2);
		temp = tail->string;
		tail->string = ft_strdup(p_newline + 1);
		if (temp)
			free(temp);
		return (result);
	}
	if (tail->last_buff)
	{
		tail->last_line = 1;
		result = ft_strdup(tail->string);
		free(tail->string);
		return (result);
	}
	return (NULL);
}

int	read_from_file(t_line *tail)
{
	tail->bytes_read = read(tail->fd, tail->buff, BUFFER_SIZE);
	if (tail->bytes_read > 0)
	{
		tail->buff[tail->bytes_read] = '\0';
		while (tail->bytes_read > 0 && !tail->has_newline)
		{
			if (tail->string == NULL)
				tail->string = ft_strdup(tail->buff);
			else
				tail->string = ft_strjoin(tail->string, tail->buff);
			if (ft_strchr(tail->buff, '\n'))
				tail->has_newline = 1;
			else
			{
				tail->bytes_read = read(tail->fd, tail->buff, BUFFER_SIZE);
				tail->buff[tail->bytes_read] = '\0';
			}
		}
	}
	return (tail->bytes_read);
}

char	*check_result(char *result, t_line **first_elem)
{
	t_line	*tail;

	tail = *first_elem;
	if (tail->last_line || tail->bytes_read < 0)
	{
		free(tail->buff);
		free(tail);
		*first_elem = NULL;
	}
	if (result != NULL && *result == '\0')
	{
		free(result);
		return (NULL);
	}
	return (result);
}

char	*get_next_line(int fd)
{
	static t_line	*tail;
	char			*result;

	if (fd < 0)
		return (NULL);
	if (NULL == tail)
		tail = create_tail(fd);
	if (read_from_file(tail) < 0)
		return (check_result(NULL, &tail));
	if (tail->bytes_read == 0)
		tail->last_buff = 1;
	result = get_line_from_string(tail);
	tail->has_newline = 0;
	if (!tail->last_buff)
		tail->buff_size = ft_strlen(tail->string);
	return (check_result(result, &tail));
}
