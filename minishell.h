/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 19:08:27 by dirony            #+#    #+#             */
/*   Updated: 2022/03/23 17:48:34 by fdarkhaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <histedit.h>
# include <string.h>
# include <errno.h>

# define SPACES " \n\t\v\f\r"
# define SEMICOLON 202
# define PIPE 204
# define AND_SIGN 201 //чётность кодов использую в парсинге. выбрасываю по 1 или по 2 символа, в зависимости от разделителя
# define OR_SIGN 203

typedef struct s_limiter
{
	int	sign;
	int	index;
}	t_limiter;

typedef struct s_info
{
	int			num_of_commands;
	t_limiter	*limiters;
	int			in_redirect;
	int			out_redirect;
	int			err_redirect;
	t_list		*commands;
}	t_info;

int		is_exit_command(char *str);
int		is_builtin_command(char *s);
void	get_info_from_string(char *s, t_info *info);

t_list	*add_cmd_to_list(int argc, char **argv, char **envp);
char	*get_cmd_name(char *cmd);
char	*get_cmd_path(char *cmd, char **envp);
void	print_cmd_error(char *argv);

t_env	*create_env(char **envp);
char	**return_env_to_char(t_env *env);
void	clear_env(t_env *list);
void	lstiter_env(t_env *list, void (*f)(void *));
void	free_str_pointer(char **str);//перенести в другой раздел
// builtin's //
int		execute_cd_command(t_list *cmd, char **envp, t_env *env);
int		execute_exit_command(t_list *cmd, char **envp);
int		execute_echo_command(t_list *cmd, char **envp);
int		execute_pwd_command(t_list *cmd, char **envp);
int		execute_env_command(t_list *cmd, char **envp);
int		execute_unset_command(t_list *cmd, char **envp, t_env *env);
int		execute_export_command(t_list *cmd, char **envp, t_env *env);
void	env_lstadd_back(t_env **list, t_env *new_elem);
t_env	*env_create_elem(char *str);

void	execute_with_redirect(int *fd, t_list *list, char **envp);

#endif