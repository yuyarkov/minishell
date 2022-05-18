/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 20:26:34 by dirony            #+#    #+#             */
/*   Updated: 2022/05/18 20:26:43 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	dup_child_pipe(t_list *cmd)
{
	close(cmd->end[0]);
	if (dup2(cmd->previous->end[0], STDIN_FILENO) < 0)
		perror ("Could not dup2 STDIN");
	close(cmd->previous->end[0]);
	if (dup2(cmd->end[1], STDOUT_FILENO) < 0)
		perror ("Could not dup2 STDOUT");
	close (cmd->end[1]);
}

void	dup_redirect_in_for_cmd(t_list *cmd)
{
	if (cmd->redirect_in == REDIRECT_IN)
	{
		cmd->fd[0] = open(cmd->redirect_in_file, O_RDONLY);
		if (cmd->fd[0] < 0)
			print_file_error(cmd->redirect_in_file);
	}
	if (cmd->redirect_in == REDIRECT_HEREDOC)
		pipe_for_heredoc(cmd, cmd->fd);
	cmd->fd[2] = dup(STDIN_FILENO);
	if (dup2(cmd->fd[0], STDIN_FILENO) < 0)
		perror ("Could not dup2 STDOUT");
	close (cmd->fd[0]);
}

void	dup_redirect_out_for_cmd(t_list *cmd)
{
	if (cmd->redirect_out == REDIRECT_OUT)
		cmd->fd[1] = open(cmd->redirect_out_file, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (cmd->redirect_out == REDIRECT_APPEND)
		cmd->fd[1] = open(cmd->redirect_out_file, O_CREAT | O_RDWR | O_APPEND, 0644);	
	if (cmd->fd[1] < 0)
		print_file_error(cmd->redirect_out_file);
	cmd->fd[3] = dup(STDOUT_FILENO);
	if (dup2(cmd->fd[1], STDOUT_FILENO) < 0)
		perror ("Could not dup2 STDOUT");
	close (cmd->fd[1]);
}

void	dup_back_redirect(t_list *cmd)
{
	printf("inside dup_back, fd[0]: %d, fd[1]: %d\n", cmd->fd[0], cmd->fd[1]);
	if (cmd->redirect_in)
	{
		if (dup2(cmd->fd[2], STDIN_FILENO) < 0)
			perror ("Could not dup2 STDIN");
	}
	if (cmd->redirect_out)
	{		
		if (dup2(cmd->fd[3], STDOUT_FILENO) < 0)
			perror ("Could not dup2 STDOUT");
	}
}

void	child_pipex(t_list *cmd, t_info *info)
{
	if (!cmd->previous)
	{
		close(cmd->end[0]);
		if (cmd->redirect_in)
			dup_redirect_in_for_cmd(cmd);
		if (dup2(cmd->end[1], STDOUT_FILENO) < 0)
			perror ("Could not dup2 STDOUT");
		close (cmd->end[1]);
	}
	else if (cmd->next)
		dup_child_pipe(cmd);
	else
	{
		close (cmd->end[1]);//добавил, почему-то раньше не было
		if (dup2(cmd->previous->end[0], STDIN_FILENO) < 0)
			perror ("Could not dup2 STDIN");
		close (cmd->previous->end[0]);
		if (cmd->redirect_out)
			dup_redirect_out_for_cmd(cmd);
	}
	if (is_builtin_command(cmd->cmd))
		execute_builtin(cmd, info->envp, info);
	else if (execve(cmd->cmd, cmd->arguments, info->envp) == -1)
		perror ("Could not execve");
	exit(EXIT_FAILURE);//тут видимо надо поменять код exit
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

t_list	*execute_with_pipe(t_list *list, t_info *info)
{
	int		status;
	pid_t	child;
	t_list	*iter;

	iter = list;
	while (iter && (iter->limiter == PIPE || (iter->previous && iter->previous->limiter == PIPE)))
	{
		if (iter->next)
			pipe(iter->end);
		child = fork();
		if (child < 0)
			exit(EXIT_FAILURE);//придумать корректный выход
			//return (perror("Fork: "));
		if (child == 0)
			child_pipex(iter, info);
		close_parent_pipes(iter);
		if (!iter->next)
			waitpid(child, &status, 0);
		info->status = status / 256;
		iter = iter->next;
	}
	return (iter);
}
