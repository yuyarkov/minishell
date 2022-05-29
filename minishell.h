/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 19:08:27 by dirony            #+#    #+#             */
/*   Updated: 2022/05/29 12:03:41 by dirony           ###   ########.fr       */
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
# define AND_SIGN 201
# define SEMICOLON 202
# define OR_SIGN 203
# define PIPE 204
# define WORD 205
# define QUOTE 206
# define DOUBLE_QUOTE 207
# define REDIRECT_OUT 208
# define REDIRECT_IN 209
# define REDIRECT_APPEND 210
# define REDIRECT_HEREDOC 211
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

typedef struct s_token
{
	int				type;
	char			*value;
	int				level;
	int				group;
	int				in_qoutes;
	int				status;
	struct s_token	*left;
	struct s_token	*right;
}	t_token;

typedef struct s_info
{
	t_token		*tokens;
	int			num_of_tokens;
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

void	parse_and_execute_string(char *s, t_info *info);
int		is_builtin_command(char *s);
int		has_double_special_symbol(char *s);
int		put_word_token(char *s, t_token *token, int *k);
int		put_special_token(char *s, t_token *token, int *k);
int		put_dollar_key_to_token(char *s, t_token *t, int *k);
int		put_double_special_token(char *s, t_token *token, int *k);
int		put_token_from_single_quotes(char *s, t_token *t, int *k);
int		put_token_from_double_quotes(char *s, t_token *t, int *k);

int		check_bad_syntax(t_info *info);

void	put_tree_level_marks(t_info *info);
void	put_group_id_marks(t_info *info);
void	put_tree_marks(t_info *info, t_token *t);
int		is_limiter(t_token t);
t_token	*get_next_limiter(t_token *token, t_info *info);
int		parse_and_execute_tree(t_info *info);
void	join_words_inside_quotes(t_token *t);
void	check_and_replace_dollar(t_token *t, t_info *info);
void	get_argv_from_token(t_token *t, t_info *info, t_list *cmd, int group);
void	get_redirect_from_token(t_token *t, t_list *cmd);
int		execute_group(t_list *cmd, char **envp, t_info *info);
void	execute_builtin(t_list *cmd, char **envp, t_info *info);

void	dup_redirect_in_for_cmd(t_list *cmd);
void	dup_redirect_out_for_cmd(t_list *cmd);
void	dup_back_redirect(t_list *cmd);
char	*get_next_line(int fd);
int		read_from_heredoc(t_list *cmd, int *end);
void	pipe_for_heredoc(t_list *iter, int *fd);
void	child_pipex(t_list *cmd, t_info *info);

t_list	*add_cmd_to_list(t_info *info, char **argv, char **envp);
void	ft_double_list_add_back(t_list **list, t_list *new_elem);
char	*get_first_word(char *cmd);
char	*get_cmd_path(char *cmd, t_info *info);
char	*find_cmd_path(char *cmd, char *path);
void	print_cmd_error(char *argv, int *status);
void	print_file_error(char *file_name);

t_list	*create_elem_cmd(t_token *t, t_info *info);
t_token	*get_next_root_limiter(t_token *token, t_info *info);
void	get_redirect_from_token(t_token *t, t_list *c);
void	get_argv_from_token(t_token *t, t_info *info, t_list *cmd, int group);

t_env	*create_env(char **envp, int ac, char **av);
char	**return_env_to_char(t_env *env);
void	clear_env(t_env *list);
void	lstiter_env(t_env *list, void (*f)(void *));
void	free_string_array(char **str);
void	free_befor_exit(char *str, char **split_arg, t_info *info);
void	clear_tokens(t_info *info);
void	clear_info(t_info *info);
void	clear_cmd(t_list *iter);
void	clear_info_except_envp(t_info *info);
void	clear_list_env(t_env *env);
char	*get_dollar_value_from_env(char *s, t_info *info);

int		execute_commands(t_list *commands, char **envp, t_env **env);
void	execute_cmd(t_list *cmd, char **envp);

// builtin's //
int		execute_cd_command(t_list *cmd, char **envp, t_env *env);
int		is_exit_command(char *str, t_info *info);
void	execute_exit_command(t_list *cmd);
int		execute_echo_command(t_list *cmd);
int		execute_pwd_command(t_list *cmd);
int		execute_unset_command(t_list *cmd, t_env *env);
int		execute_export_command(t_list *cmd, t_env *env);
void	env_lstadd_back(t_env **list, t_env *new_elem);
t_env	*env_create_elem(char *str);
int		add_arguments(t_list *cmd, t_env **env);
void	create_sort_env(t_env *env);

int		execute_with_pipe(t_list *list, t_info *info);

void	ft_signal(int i);
void	if_value_is_null(void *value);
void	if_pointer_is_null(char **value);
int		print_error_token(t_info *info, int token);

void	print_commands_list(t_list *cmd);
void	print_tokens(t_info *info);

#endif
