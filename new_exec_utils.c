/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_exec_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 11:48:38 by dirony            #+#    #+#             */
/*   Updated: 2022/05/03 14:58:44 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_cmd_from_group(t_list *cmd, char **envp)
{
	
	ft_signal(2);
	printf("вот такую команду исполняю: %s, argv: %p\n", cmd->cmd, cmd->arguments);
	printf("флаги resirect_in: %d, redirect_out: %d\n", cmd->redirect_in, cmd->redirect_out);

	if (cmd->redirect_in)
	{
		printf("пытаюсь открыть файл: %s\n", cmd->redirect_in_file);
		cmd->fd[0] = open(cmd->redirect_in_file, O_RDONLY);//закрыть либо в этой функции, либо в вызывающей
		if (cmd->fd[0] < 0)
			print_file_error(cmd->redirect_in_file);
		dup2(cmd->fd[0], STDIN_FILENO);
	}
	if (cmd->redirect_out)
	{
		cmd->fd[1] = open(cmd->redirect_out_file, O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (cmd->fd[1] < 0)
			print_file_error(cmd->redirect_out_file);
		dup2(cmd->fd[1], STDOUT_FILENO);
	}
	if (execve(cmd->cmd, cmd->arguments, envp) == -1)
		perror ("Could not execve");
	// exit(EXIT_SUCCESS);//подумать, как брать корректный код выхода из execve
	exit(EXIT_SUCCESS);//При успешном завершении execve() не возвращает управление - EXIT_FAILURE
}

int	execute_group(t_list *cmd, char **envp, t_env **env)
{
	pid_t	child;
	int		status;

	status = 0;
	if (is_builtin_command(cmd->cmd))
		status = execute_builtin(cmd, envp, env);
	else if (cmd->cmd && *cmd->cmd != '\0')
	{
		if (cmd->limiter == PIPE)
			cmd = execute_with_pipe(cmd, envp);
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
				execute_cmd_from_group(cmd, envp);
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
	return (status);
}