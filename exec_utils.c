/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 20:26:34 by dirony            #+#    #+#             */
/*   Updated: 2022/02/18 20:51:06 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	dup_child_pipe(t_list *cmd)
{
	close(cmd->end[0]);
	if (dup2(cmd->previous->end[0], STDIN_FILENO) < 0)
		perror ("Could not dup2 STDIN");
	if (dup2(cmd->end[1], STDOUT_FILENO) < 0)
		perror ("Could not dup2 STDOUT");
}

void	child_pipex(int *fd, t_list *cmd, char **envp)
{
	if (!cmd->previous)
	{
		close(cmd->end[0]);
		if (dup2(fd[0], STDIN_FILENO) < 0)
			perror ("Could not dup2 STDIN");
		if (dup2(cmd->end[1], STDOUT_FILENO) < 0)
			perror ("Could not dup2 STDOUT");
	}
	else if (cmd->next)
		dup_child_pipe(cmd);
	else
	{
		if (dup2(cmd->previous->end[0], STDIN_FILENO) < 0)
			perror ("Could not dup2 STDIN");
		if (dup2(fd[1], STDOUT_FILENO) < 0)
			perror ("Could not dup2 STDOUT");
	}
	if (execve(cmd->cmd, cmd->arguments, envp) == -1)
		perror ("Could not execve");
	exit(EXIT_FAILURE);
}

void	close_parent_pipes(t_list *iter)
{
	if (!iter->previous)
		close(iter->end[1]);
	else if (iter->next)
	{
		close(iter->end[1]);
		close(iter->previous->end[0]);
	}
	else
		close(iter->previous->end[1]);
}

void	execute_with_redirect(int *fd, t_list *list, char **envp)
{
	int		status;
	pid_t	child;
	t_list	*iter;

	printf("inside execute_with_redirect\n");
	iter = list;
	// if (list->redirect)
	// 	pipe_for_heredoc(list, &(fd[0]));//это была для ввода с heredoc
	while (iter)
	{
		if (iter->next)
			pipe(iter->end);
		child = fork();
		if (child < 0)
			return (perror("Fork: "));
		if (child == 0)
			child_pipex(fd, iter, envp);
		close_parent_pipes(iter);
		waitpid(child, &status, 0);
		iter = iter->next;
	}
}