/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_exec_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 11:48:38 by dirony            #+#    #+#             */
/*   Updated: 2022/05/13 20:33:58 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_cmd_from_group(t_list *cmd, char **envp, t_info *info)
{
	ft_signal(2);
	int	status;

	(void) info;
	status = 0;
	if (cmd->redirect_in)
		dup_redirect_in_for_cmd(cmd);
	if (cmd->redirect_out)
		dup_redirect_out_for_cmd(cmd);
	status = execve(cmd->cmd, cmd->arguments, envp);
	if (status == -1)
			perror ("Could not execve");
	exit(status);//При успешном завершении execve() не возвращает управление
}

int	execute_group(t_list *commands, char **envp, t_info *info)
{
	pid_t	child;
	int		status;
	t_list	*iter;

	if (commands)
		status = 0;
	else
		status = 127; //разобраться почему 127 и заменить на константу
							//print_commands_list(commands);
	iter = commands;
	while (iter)
	{
		if (iter->cmd && *iter->cmd != '\0')
		{
			if (iter->limiter == PIPE)
			{
				iter = execute_with_pipe(iter, info);
				return (info->status);//считаю, что после пайпов в группе уже нет команд и выхожу
			}
			else if (is_builtin_command(iter->cmd))
				execute_builtin(iter, info->envp, info);
			else
			{
				child = fork();
				if (child < 0)
				{
					perror("Fork: ");
					return (-1);//подумать, какой правильный код возвращать
				}
				signal(SIGINT, SIG_IGN);//сигнал SIGINT игнорируется
				signal(SIGQUIT, SIG_IGN);//сигнал SIGQUIT игнорируется
				if (child == 0)
					execute_cmd_from_group(iter, envp, info);
				waitpid(child, &status, WUNTRACED);
				if (WIFSIGNALED(status))//если во время дочернего процесса (cat или grep) передан сигнал
				{
					if (WTERMSIG(status) == 3)
						ft_putstr_fd("Quit: 3\n", 1);// back slash
					else
						ft_putstr_fd("\n", 1);// ^C
				}
			}
		}
		if (iter)
			iter = iter->next;
	}
	info->status = status;
	//printf("-------присвоил статус: %c, char: %d\n", status, info->status);
	return (status);
}
