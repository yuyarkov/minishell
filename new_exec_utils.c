/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_exec_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 11:48:38 by dirony            #+#    #+#             */
/*   Updated: 2022/05/29 13:58:07 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_cmd_from_group(t_list *cmd, char **envp, t_info *info)
{
	int	status;

	ft_signal(2);
	(void) info;
	status = 0;
	if (cmd->redirect_in)
		dup_redirect_in_for_cmd(cmd);
	if (cmd->redirect_out)
		dup_redirect_out_for_cmd(cmd);
	status = execve(cmd->cmd, cmd->arguments, envp);
	if (status == -1)
		perror ("Could not execve");
	exit(status);
}

int	execute_child_cmd(t_list *iter, char **envp, t_info *info)
{
	pid_t	child;
	int		status;	

	child = fork();
	if (child < 0)
	{
		perror("Fork: ");
		return (-1);
	}
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	if (child == 0)
		execute_cmd_from_group(iter, envp, info);
	waitpid(child, &status, WUNTRACED);
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == 3)
			ft_putstr_fd("Quit: 3\n", 1);
		else
			ft_putstr_fd("\n", 1);
	}
	return (status);
}

int	execute_group(t_list *commands, char **envp, t_info *info)
{
	int		status;
	t_list	*iter;

	if (commands)
		status = 0;
	else
		status = 127;
	iter = commands;
	while (iter)
	{
		if (iter->cmd && *iter->cmd != '\0')
		{
			if (iter->limiter == PIPE)
				return (execute_with_pipe(iter, info));
			else if (is_builtin_command(iter->cmd))
				execute_builtin(iter, info->envp, info);
			else
				status = execute_child_cmd(iter, envp, info);
		}
		if (!iter->cmd)
			return (EXIT_FAILURE);
		iter = iter->next;
	}
	info->status = status / 256;
	return (status);
}
