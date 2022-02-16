/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 19:55:35 by dirony            #+#    #+#             */
/*   Updated: 2022/02/16 21:07:44 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_cmd_name(char *cmd)
{
	char	*result;
	int		i;

	i = 0;
	result = malloc(ft_strlen(cmd) + 1);
	if (NULL == result)
		return (NULL);
	while (cmd[i] && cmd[i] != ' ')
	{
		result[i] = cmd[i];
		i++;
	}
	result[i] = '\0';
	return (result);
}

char	*find_cmd_path(char *cmd, char *path)
{
	char	**dirs;
	char	*temp;
	int		i;
	char	*result;

	dirs = ft_split(path, ':');
	i = 0;
	result = NULL;
	temp = get_cmd_name(cmd);
	cmd = ft_strjoin("/", temp);
	free(temp);
	while (dirs[i] && !result)
	{
		temp = ft_strjoin(dirs[i], cmd);
		if (access(temp, 1 << 0) == 0)
			result = ft_strdup(temp);
		free(temp);
		free(dirs[i]);
		i++;
	}
	while (dirs[i])
		free(dirs[i++]);
	free(dirs);
	free(cmd);
	return (result);
}

char	*get_cmd_path(char *cmd, char **envp)
{
	int		i;
	int		j;
	char	start[5];
	int		result;

	result = 0;
	i = 0;
	while (!result && envp[i])
	{
		j = 0;
		while (j < 5)
		{
			start[j] = envp[i][j];
			j++;
		}
		if (ft_strncmp("PATH=", start, 5) == 0)
			result = i;
		i++;
	}
	if (result != 0)
		return (find_cmd_path(cmd, envp[result] + 5));
	else
		return (NULL);
}
