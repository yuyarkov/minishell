/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/09 17:03:52 by dirony            #+#    #+#             */
/*   Updated: 2022/04/09 17:04:29 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_cmd(t_list *cmd, char **envp)
{
	//printf("вот такую команду исполняю: %s\n", cmd->cmd);
	if (execve(cmd->cmd, cmd->arguments, envp) == -1)
		perror ("Could not execve");
	exit(EXIT_SUCCESS);//подумать, как брать корректный код выхода из execve
}

int	execute_builtin(t_list *cmd, char **envp, t_env **env)
{
	if (ft_strncmp(cmd->cmd, "cd", 2) == 0)
		return (execute_cd_command(cmd, envp, *env));
	if (ft_strncmp(cmd->cmd, "exit ", 5) == 0)
		return (execute_exit_command(cmd, envp));
	if (ft_strncmp(cmd->cmd, "echo ", 5) == 0)
		return (execute_echo_command(cmd, envp));
	if (ft_strncmp(cmd->cmd, "pwd", 3) == 0)
		return (execute_pwd_command(cmd, envp));
	// if (ft_strncmp(cmd->cmd, "env\0", 4) == 0)
	// 	return (execute_env_command(cmd, envp));
	if (ft_strncmp(cmd->cmd, "unset\0", 6) == 0)
		return (execute_unset_command(cmd, envp, *env));
	if (ft_strncmp(cmd->cmd, "export\0", 7) == 0)
		return (execute_export_command(cmd, envp, *env));
	return (0);
}

int	execute_commands(t_list *commands, char **envp, t_env **env)
{
	pid_t	child;
	int		status;
	t_list	*iter;

	if (commands)
		status = 0;
	else
		status = 127; //разобраться почему 127 и заменить на константу
	iter = commands;
	while (iter)
	{
		if (is_builtin_command(iter->cmd))
		{
			status = execute_builtin(iter, envp, env);
			// printf("1 status = %d\n", status);
		}
		else if (iter->cmd && *iter->cmd != '\0')
		{
		//	printf("iter->cmd: %s, iter->limiter: %d, iter->previous: %p\n", iter->cmd,
		//		 iter->limiter, iter->previous);
			if (iter->limiter == PIPE)
				iter = execute_with_pipe(iter, envp);
			else
			{
				child = fork();
				if (child < 0)
				{
					perror("Fork: ");
					return (-1);//подумать, какой правильный код возвращать
				}
				if (child == 0)
					execute_cmd(iter, envp);
				// printf("2 status = %d\n", status);
				waitpid(child, &status, 0);
			}
		}
		if (iter)
			iter = iter->next;
	}
	// printf("3 status = %d\n", status);
	return (status);
}