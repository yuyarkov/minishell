/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/25 15:57:12 by dirony            #+#    #+#             */
/*   Updated: 2022/05/27 18:26:26 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pipe_for_heredoc(t_list *iter, int *fd)
{
	int		status;
	pid_t	child;
	int		end[2];

	pipe(end);
	child = fork();
	if (child < 0)
		return (perror("Fork: "));
	if (child == 0)
		read_from_heredoc(iter, end);
	close(end[1]);
	waitpid(child, &status, 0);
	*fd = end[0];
}

int	read_from_heredoc(t_list *cmd, int *end)
{
	char	*buff;
	char	*limiter;
	char	*temp;
	int		lim_len;

	close(end[0]);
	limiter = ft_strjoin(cmd->heredoc_eof, "\n");
	lim_len = ft_strlen(limiter);
	ft_putstr_fd("> ", 1);
	buff = get_next_line(0);
	while (ft_strncmp(buff, limiter, lim_len) != 0)
	{
		temp = buff;
		ft_putstr_fd(buff, end[1]);
		ft_putstr_fd("> ", 1);
		buff = get_next_line(0);
		free(temp);
	}
	free(buff);
	free(limiter);
	exit(EXIT_SUCCESS);
}
