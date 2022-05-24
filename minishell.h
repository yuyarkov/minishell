/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 19:08:27 by dirony            #+#    #+#             */
/*   Updated: 2022/05/24 20:52:50 by dirony           ###   ########.fr       */
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

# define BUFFER_SIZE 4096
# define SHELL "minishell$ "
# define SPACES " \n\t\v\f\r"
# define SPECIAL_SYMBOLS ";|\'\"><()\\$&|"
# define VALID_KEY_SYMBOLS ""
# define EOF_TOKENS -600
# define NEVER_EXECUTED -500
# define AND_SIGN 201 //чётность кодов использую в парсинге. выбрасываю по 1 или по 2 символа, в зависимости от разделителя
# define SEMICOLON 202// ;
# define OR_SIGN 203
# define PIPE 204
# define WORD 205
# define QUOTE 206
# define DOUBLE_QUOTE 207
# define REDIRECT_OUT 208// > перенаправление вывода с перезаписью
# define REDIRECT_IN 209// < перенаправление ввод из файла с перезаписью
# define REDIRECT_APPEND 210// >> перенаправление вывода с добавлением
# define REDIRECT_HEREDOC 211// << перенаправление ввода из файла с добавлением
# define LEFT_PARENTHESIS 212
# define RIGHT_PARENTHESIS 213
# define DOLLAR_SIGN 215
# define DOLLAR_KEY 216
# define BACKSLASH 214
# define OUTPUT_FILE 300
# define INPUT_FILE 301
# define HEREDOC_EOF 302
# define ARGV 303
# define CMD 305
# define NEW_LINE 1000

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
	int				group;
	int				in_qoutes;//чтобы объединять в один аргумент содержимое кавычек
	int				status;//по умолчанию будем присваивать NEVER_EXECUTED
	struct s_token	*left;
	struct s_token	*right;
	struct s_token	*root;//пока непонятно, нужен ли
}	t_token;

typedef struct s_info
{
	int			num_of_commands;//не пригодилось, убрать, как уберу файл parse_utils
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
	int			changed_envp;
}	t_info;

typedef struct s_line
{
	char			*string;
	char			*buff;
	size_t			buff_size;
	int				fd;
	int				bytes_read;
	int				has_newline;
	int				last_buff;
	int				last_line;
}	t_line;

int		is_exit_command(char *str);

void	get_tokens_from_string(char *s, t_info *info);

int		is_builtin_command(char *s);
int		has_double_special_symbol(char *s);
int		put_word_token(char *s, t_token *token, int *k);
int		put_special_token(char *s, t_token *token, int *k);
int		put_dollar_key_to_token(char *s, t_token *t, int *k);
int		put_double_special_token(char *s, t_token *token, int *k);
int		put_token_from_single_quotes(char *s, t_token *t, int *k);
int		put_token_from_double_quotes(char *s, t_token *t, int *k);

int		check_bad_syntax(t_info *info);
//void	parse_commands(char *str, t_info *info, char **envp);

void	put_tree_level_marks(t_info *info);
void	put_group_id_marks(t_info *info);
void	put_tree_marks(t_info *info);
int		is_limiter(t_token t);
t_token	*get_next_limiter(t_token *token, t_info *info);
int		parse_and_execute_tree(t_info *info);
void	get_argv_from_token(t_token *t, t_info *info, t_list *cmd);
void	get_redirect_from_token(t_token *t, t_info *info, t_list *cmd);
int		execute_group(t_list *cmd, char **envp, t_info *info);
int		execute_builtin(t_list *cmd, char **envp, t_info *info);

void	dup_redirect_in_for_cmd(t_list *cmd);
void	dup_redirect_out_for_cmd(t_list *cmd);
void	dup_back_redirect(t_list *cmd);
char	*get_next_line(int fd);
int		read_from_heredoc(t_list *cmd, int *end);
void	pipe_for_heredoc(t_list *iter, int *fd);

t_list	*add_cmd_to_list(t_info *info, char **argv, char **envp);
void	ft_double_list_add_back(t_list **list, t_list *new_elem);
char	*get_first_word(char *cmd);
char	*get_cmd_path(char *cmd, char **envp, t_info *info);
void	print_cmd_error(char *argv);
void	print_file_error(char *file_name);

t_env	*create_env(char **envp);
char	**return_env_to_char(t_env *env);
void	clear_env(t_env *list);
void	lstiter_env(t_env *list, void (*f)(void *));
void	free_string_array(char **str);//перенести в другой раздел
void	clear_tokens(t_info *info);
void	clear_info(t_info *info);//зачаток общей функции, которая чистит всё
void	clear_info_except_envp(t_info *info);
void	clear_list_env(t_env *env);//зачаток общей функции, которая чистит всё
char	*get_dollar_value_from_env(char *s, t_info *info);

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
int		max_strlen(char *str1, char *str2);
t_env	*if_arg_is_not_in_env(char *arg, t_env **env);
int		find_argument_in_env(char *argument, t_env **env);
void	if_there_is_equal_sign(char *str, t_env **new_elem);
int		find_key_in_list(char *argument, t_env *list);
void	print_export(t_env *env);
void	change_strmin(t_env **strmin, t_env *tmp);
// t_env	*sort_env(t_env *env);////del?

t_list	*execute_with_pipe(t_list *list, t_info *info);

void	ft_signal(int i);
int		print_error_token(t_info *info, int token);
void	free_after_ctrl_d(char *str, t_info *info);
void	if_value_is_null(void *value);// проверка на возврат malloc'ом NULL
void	if_pointer_is_null(char **value);// проверка на NULL

void	print_commands_list(t_list *cmd);//для дебага, убрать перед сдачей вместе с файлом print_utils
void	print_tokens(t_info *info);//для дебага, убрать перед сдачей

#endif