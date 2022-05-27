/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/09 17:03:52 by dirony            #+#    #+#             */
/*   Updated: 2022/05/27 21:14:34 by fdarkhaw         ###   ########.fr       */
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

// int	execute_commands(t_list *commands, char **envp, t_env **env)
// {
// 	pid_t	child;
// 	int		status;
// 	t_list	*iter;

// 	if (commands)
// 		status = 0;
// 	else
// 		status = 127; //разобраться почему 127 и заменить на константу
// 	iter = commands;
// 	while (iter)
// 	{
// 		if (is_builtin_command(iter->cmd))
// 			status = execute_builtin(iter, envp, env);
// 		else if (iter->cmd && *iter->cmd != '\0')
// 		{
// 		//	printf("iter->cmd: %s, iter->limiter: %d, iter->previous: %p\n", iter->cmd,
// 		//		 iter->limiter, iter->previous);
// 			if (iter->limiter == PIPE)
// 				iter = execute_with_pipe(iter, envp);
// 			else
// 			{
// 				child = fork();
// 				if (child < 0)
// 				{
// 					perror("Fork: ");
// 					return (-1);//подумать, какой правильный код возвращать
// 				}
// 				signal(SIGINT, SIG_IGN);//сигнал SIGINT игнорируется
// 				signal(SIGQUIT, SIG_IGN);//сигнал SIGQUIT игнорируется
// 				if (child == 0)
// 					execute_cmd(iter, envp);
// 				waitpid(child, &status, WUNTRACED);
// 				if (WIFSIGNALED(status))//если во время дочернего процесса (cat или grep) передан сигнал
// 				{
// 					if (WTERMSIG(status) == 3)
// 						ft_putstr_fd("Quit: 3\n", 1);// back slash
// 					else
// 						ft_putstr_fd("\n", 1);// ^C
// 				}
// 			}
// 		}
// 		if (iter)
// 			iter = iter->next;
// 	}
// 	return (status);
// }
