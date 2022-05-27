/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/09 17:03:52 by dirony            #+#    #+#             */
/*   Updated: 2022/05/27 21:48:29 by fdarkhaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_cmd(t_list *cmd, char **envp)
{
	//int	i;
	ft_signal(2);
	// i = 0;
	// while (cmd->arguments && cmd->arguments[i])
	// 	{
	// 		printf("argv[%d]: %s\n", i, cmd->arguments[i]);
	// 		i++;
	// 	}
	if (execve(cmd->cmd, cmd->arguments, envp) == -1)
		perror ("Could not execve");
	exit(EXIT_FAILURE);
}

void	execute_builtin(t_list *cmd, char **envp, t_info *info)
{
	if (cmd->redirect_in)
		dup_redirect_in_for_cmd(cmd);
	if (cmd->redirect_out)
		dup_redirect_out_for_cmd(cmd);
	info->status = 0;
	if (ft_strncmp(cmd->cmd, "cd", 2) == 0)
		info->status = execute_cd_command(cmd, envp, info->env);
	else if (ft_strncmp(cmd->cmd, "exit", 5) == 0)
		info->status = execute_exit_command(cmd);
	else if (ft_strncmp(cmd->cmd, "echo", 5) == 0)
		info->status = execute_echo_command(cmd);
	else if (ft_strncmp(cmd->cmd, "pwd", 3) == 0)
		info->status = execute_pwd_command(cmd);
	else if (ft_strncmp(cmd->cmd, "unset\0", 6) == 0)
		info->status = execute_unset_command(cmd, info->env);
	else if (ft_strncmp(cmd->cmd, "export\0", 7) == 0)
		info->status = execute_export_command(cmd, info->env);
	if (cmd->redirect_in || cmd->redirect_out)
		dup_back_redirect(cmd);
	// info->status = status;
	printf("inside execute_builtin, status: %d\n", info->status);
	// return (status);
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
	while (iter && (iter->limiter == PIPE
			|| (iter->previous && iter->previous->limiter == PIPE)))
	{
		if (iter->next)
			pipe(iter->end);
		child = fork();
		if (child < 0)
			exit(EXIT_FAILURE);
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
