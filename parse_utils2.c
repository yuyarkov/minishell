/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/27 20:08:55 by dirony            #+#    #+#             */
/*   Updated: 2022/05/27 20:09:12 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_path_value_from_env(t_info *info)
{
	t_env	*iter;
	char	*result;

	result = NULL;
	iter = info->env;
	while (iter)
	{
		if (ft_strncmp(iter->key, "PATH", 5) == 0)
			result = ft_strdup(iter->value);
		iter = iter->next;
	}
	return (result);
}

char	*get_cmd_path(char *input_cmd, t_info *info)
{
	char	*cmd;
	char	*path;

	cmd = ft_strdup(input_cmd);
	if (cmd == '\0' || cmd == NULL)
		return (cmd);
	if (access(cmd, 1 << 0) == 0)
		return (cmd);
	if (is_builtin_command(cmd))
		return (cmd);
	path = get_path_value_from_env(info);
	if (path)
		return (find_cmd_path(cmd, path));
	else
		return (NULL);
}
