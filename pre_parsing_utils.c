/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_parsing_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 20:57:31 by fdarkhaw          #+#    #+#             */
/*   Updated: 2022/05/24 20:48:41 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_dollar_value_from_env(char *s, t_info *info)
{
	t_env	*iter;
	char	*result;

	if (*s == '?')
		return (ft_itoa(info->status));
	result = NULL;
	iter = info->env;
	while (iter)
	{
		if (ft_strncmp(iter->key, s, ft_strlen(iter->key)) == 0)
			result = ft_strdup(iter->value);
		iter = iter->next;
	}
	return (result);
}

void	check_and_replace_dollar(t_token *t, t_info *info)
{
	int		group;
	int		i;
	char	*temp;

	group = t->group;
	i = 0;
	while (t && t[i].type != EOF_TOKENS && t[i].group == group)
	{
		if (t[i].type == DOLLAR_KEY)
		{
			temp = t[i].value;
			t[i].value = get_dollar_value_from_env(t[i].value, info);
			t[i].type = WORD;
			free(temp);
		}
		i++;
	}
}

char	*join_word_chain(t_token *t, int *inc)
{
	int		gr;
	int		i;	
	char	*result;
	char	*temp;
	
	gr = t->group;
	result = NULL;
	i = *inc;
	while (t[i].in_qoutes && t[i].type != EOF_TOKENS && t[i].group == gr)
	{
		temp = result;
		result = ft_strjoin(result, t[i].value);
		free(temp);
		t[i].type = ARGV;
		i++;
	}
	*inc = i;
	return (result); 	
}

void	join_words_inside_quotes(t_token *t)
{
	int		gr;
	int		i;
	int		k;
	char	*result;
	char	*temp;

	gr = t->group;
	i = 0;
	result = t[i].value;
	while (t && t[i].type != EOF_TOKENS && t[i].group == gr)
	{
		while (!t[i].in_qoutes && t[i].type != EOF_TOKENS && t[i].group == gr)
			i++;
		if (t[i].in_qoutes && t[i].type != EOF_TOKENS && t[i].group == gr)
		{
			k = i;
			temp = t[k].value;
			t[k].value = join_word_chain(t, &i);
			t[k].type = WORD;
			free(temp);
		}
	}
}

void	get_argv_from_token(t_token *t, t_info *info, t_list *cmd)
{
	int		group;
	int		i;
	int		k;
	char	**result;

	check_and_replace_dollar(t, info);
	//check_and_replace_asterisk(t, info); //замена звёздочки на строку
	join_words_inside_quotes(t);
	result = malloc(sizeof(char *) * (info->num_of_tokens + 1));
	if (NULL == result)
		exit(EXIT_FAILURE);
	group = t->group;
	result[0] = ft_strdup(cmd->cmd);
	if (t && t->type == PIPE)
		i = 1;
	else
		i = 0;
	k = 1;
	while (t && t[i].type != EOF_TOKENS && t[i].group == group && t[i].type != PIPE)
	{
		if (t[i].type == WORD && t[i].value && *t[i].value != '\0')
		{
			result[k] = ft_strdup(t[i].value);
			t[i].type = ARGV;
			k++;
		}
		i++;
	}
	if (t && t[i].type == PIPE)
		cmd->limiter = PIPE;
	if (result[0])
		result[k] = NULL;
	cmd->arguments = result;
}

void	get_redirect_from_token(t_token *t, t_info *info, t_list *cmd)
{
	int		group;
	int		i;

	(void) info; //пока не пригодилось, может убрать аргумент
	group = t->group;
	if (t[0].type == PIPE)
		i = 1;
	else
		i = 0;
	while (t[i].type != EOF_TOKENS && t[i].group == group && t[i].type != PIPE)
	{
		if (t[i].type == REDIRECT_IN)
		{
			if (t[i + 1].type == WORD)
			{
				t[i + 1].type = INPUT_FILE;
				cmd->redirect_in = t[i].type;
				cmd->redirect_in_file = ft_strdup(t[i + 1].value);//тут нужно делать проверку на валидность файла
			}			
		}
		if (t[i].type == REDIRECT_HEREDOC)
		{
			if (t[i + 1].type == WORD)
			{
				t[i + 1].type = HEREDOC_EOF;
				cmd->redirect_in = t[i].type;
				cmd->heredoc_eof = t[i + 1].value;
			}			
		}
		if (t[i].type == REDIRECT_OUT || t[i].type == REDIRECT_APPEND)
		{
			if (t[i + 1].type == WORD)
			{
				t[i + 1].type = OUTPUT_FILE;
				cmd->redirect_out = t[i].type;
				cmd->redirect_out_file = ft_strdup(t[i + 1].value);
				cmd->fd[1] = open(cmd->redirect_out_file, O_CREAT | O_RDWR | O_TRUNC, 0644);
				close(cmd->fd[1]);
			}	
		}
		i++;
	}
}
