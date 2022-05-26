/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_parsing_utils2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 21:05:37 by dirony            #+#    #+#             */
/*   Updated: 2022/05/26 20:46:16 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**init_argv(t_token *t, t_info *info, t_list *cmd)
{
	char	**result;

	check_and_replace_dollar(t, info);
	join_words_inside_quotes(t);
	result = malloc(sizeof(char *) * (info->num_of_tokens + 1));
	if (NULL == result)
		exit(EXIT_FAILURE);
	result[0] = ft_strdup(cmd->cmd);
	return (result);
}

void	get_argv_from_token(t_token *t, t_info *info, t_list *cmd, int group)
{
	int		i;
	int		k;
	char	**result;

	result = init_argv(t, info, cmd);
	i = 0;
	if (t && t->type == PIPE)
		i = 1;
	k = 1;
	while (t && t[i].type != EOF_TOKENS && t[i].group == group
		&& t[i].type != PIPE)
	{
		if (t[i].type == WORD && t[i].value && *t[i].value != '\0')
		{
			result[k++] = ft_strdup(t[i].value);
			t[i].type = ARGV;
		}
		i++;
	}
	if (t && t[i].type == PIPE)
		cmd->limiter = PIPE;
	if (result[0])
		result[k] = NULL;
	cmd->arguments = result;
}

void	put_redirect_in(t_token *t, t_list *cmd)
{
	if (t[0].type == REDIRECT_IN)
	{
		if (t[1].type == WORD)
		{
			t[1].type = INPUT_FILE;
			cmd->redirect_in = t[0].type;
			cmd->redirect_in_file = ft_strdup(t[1].value);
		}
	}
}

void	put_heredoc_info(t_token *t, t_list *cmd)
{
	if (t[0].type == REDIRECT_HEREDOC)
	{
		if (t[1].type == WORD)
		{
			t[1].type = HEREDOC_EOF;
			cmd->redirect_in = t[0].type;
			cmd->heredoc_eof = t[1].value;
		}
	}
}

void	get_redirect_from_token(t_token *t, t_list *c)
{
	int		group;
	int		i;

	group = t->group;
	if (t[0].type == PIPE)
		i = 1;
	else
		i = 0;
	while (t[i].type != EOF_TOKENS && t[i].group == group && t[i].type != PIPE)
	{
		put_redirect_in(t, c);
		put_heredoc_info(t, c);
		if (t[i].type == REDIRECT_OUT || t[i].type == REDIRECT_APPEND)
		{
			if (t[i + 1].type == WORD)
			{
				t[i + 1].type = OUTPUT_FILE;
				c->redirect_out = t[i].type;
				c->out_file = ft_strdup(t[i + 1].value);
				c->fd[1] = open(c->out_file, O_CREAT | O_RDWR | O_TRUNC, 0644);
				close(c->fd[1]);
			}	
		}
		i++;
	}
}
