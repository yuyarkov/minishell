/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 19:08:27 by dirony            #+#    #+#             */
/*   Updated: 2022/05/04 21:05:01 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <stdio.h>
# include <termios.h> 
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <histedit.h>
# include <string.h>
# include <errno.h>
# include <signal.h>

# define SHELL "minishell$ "
# define SPACES " \n\t\v\f\r"
# define SPECIAL_SYMBOLS ";|\'\"><()\\$&|"
# define END_OF_TOKENS -600
# define CMD 305
# define ARGV 303
# define SEMICOLON 202
# define PIPE 204
# define AND_SIGN 201 //чётность кодов использую в парсинге. выбрасываю по 1 или по 2 символа, в зависимости от разделителя
# define OR_SIGN 203
# define WORD 205
# define QUOTE 206
# define DOUBLE_QUOTE 207
# define REDIRECT_OUT 208
# define OUTPUT_FILE 300
# define REDIRECT_IN 209
# define INPUT_FILE 301
# define REDIRECT_APPEND 210
# define REDIRECT_HEREDOC 211
# define HEREDOC_EOF 302
# define LEFT_PARENTHESIS 212
# define RIGHT_PARENTHESIS 213
# define BACKSLASH 214
# define DOLLAR_SIGN 215
# define NEVER_EXECUTED -500

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_limiter
{
	int	sign;
	int	index;
}	t_limiter;

typedef struct s_token
{
	int				type;
	char			*value;
	int				level;
	int				group_id;
	int				status;//по умолчанию будем присваивать NEVER_EXECUTED
	struct s_token	*left;
	struct s_token	*right;
	struct s_token	*root;//пока непонятно, нужен ли
}	t_token;

typedef struct s_info
{
	int			num_of_commands;
	t_token		*tokens;
	int			num_of_tokens;
	t_limiter	*limiters;
	int			in_redirect;
	int			out_redirect;
	int			err_redirect;
	int			status;
	t_list		*commands;
	t_env		*env;
	char		**envp;
}	t_info;

int		is_exit_command(char *str);

void	get_tokens_from_string(char *s, t_info *info);

int		is_builtin_command(char *s);
void	get_info_from_string(char *s, t_info *info);

int		check_bad_syntax(char *str, t_info *info);
void	parse_commands(char *str, t_info *info, char **envp);

void	put_tree_level_marks(t_info *info);//подумать нужен ли int или void
void	put_group_id_marks(t_info *info);
void	put_tree_marks(t_info *info);
int		is_limiter(t_token t);
t_token	*get_next_limiter(t_token *token, t_info *info);
int		parse_and_execute_tree(t_info *info);
int		execute_group(t_list *cmd, char **envp, t_env **env);
int		execute_builtin(t_list *cmd, char **envp, t_env **env);

t_list	*add_cmd_to_list(t_info *info, char **argv, char **envp);
void	ft_double_list_add_back(t_list **list, t_list *new_elem);
char	*get_first_word(char *cmd);
char	*get_cmd_path(char *cmd, char **envp);
void	print_cmd_error(char *argv);
void	print_file_error(char *file_name);

t_env	*create_env(char **envp);
char	**return_env_to_char(t_env *env);
void	clear_env(t_env *list);
void	lstiter_env(t_env *list, void (*f)(void *));
void	free_string_array(char **str);//перенести в другой раздел
void	clear_tokens(t_info *info);
void	clear_info(t_info info);//зачаток общей функции, которая чистит всё
void	clear_list_env(t_env *env);//зачаток общей функции, которая чистит всё

int		execute_commands(t_list *commands, char **envp, t_env **env);
void	execute_cmd(t_list *cmd, char **envp);

// builtin's //
int		execute_cd_command(t_list *cmd, char **envp, t_env *env);
int		execute_exit_command(t_list *cmd, char **envp);
int		execute_echo_command(t_list *cmd, char **envp);
int		execute_pwd_command(t_list *cmd, char **envp);
int		execute_unset_command(t_list *cmd, char **envp, t_env *env);
int		execute_export_command(t_list *cmd, char **envp, t_env *env);
void	env_lstadd_back(t_env **list, t_env *new_elem);
t_env	*env_create_elem(char *str);
int		find_max_strlen(char *str1, char *str2);

t_list	*execute_with_pipe(t_list *list, char **envp);

void	ft_signal(int i);

void	print_commands_list(t_list *cmd);//для дебага, убрать перед сдачей вместе с файлом print_utils
void	print_tokens(t_info *info);//для дебага, убрать перед сдачей

#endif