/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 19:55:35 by dirony            #+#    #+#             */
/*   Updated: 2022/05/28 13:55:48 by fdarkhaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	is_exit_command(char *str, t_info *info)
// {
// 	if (!str)
// 		return (0);
// 	if (ft_strncmp(str, "exit\0", 5) == 0)
// 	{
// 		clear_info(info);
// 		return (1);
// 	}
// 	else
// 		return (0);
// }

int	is_builtin_command(char *s)
{
	if (!s)
		return (0);
	if (ft_strncmp(s, "exit", 5) == 0)
		return (1);
	if (ft_strncmp(s, "cd", 3) == 0)
		return (1);
	if (ft_strncmp(s, "echo", 5) == 0)
		return (1);
	if (ft_strncmp(s, "pwd", 3) == 0)
		return (1);
	if (ft_strncmp(s, "unset", 6) == 0)
		return (1);
	if (ft_strncmp(s, "export", 7) == 0)
		return (1);
	return (0);
}

char	*get_first_word(char *cmd)
{
	char	*result;
	int		i;

	if (!cmd)
		return (NULL);
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

char	*iterate_path(char **dirs, char *cmd, char *temp_cmd)
{
	int		i;
	char	*result;
	char	*temp;

	i = 0;
	result = NULL;
	while (dirs[i] && !result)
	{
		if (ft_strncmp(dirs[i], temp_cmd, ft_strlen(dirs[i])) != 0)
			temp = ft_strjoin(dirs[i], temp_cmd);
		else
			temp = ft_strdup(temp_cmd);
		if (access(temp, 1 << 0) == 0)
			result = ft_strdup(temp);
		free(temp);
		i++;
	}
	if (cmd == temp_cmd)
		free(temp_cmd);
	else
	{
		free(cmd);
		free(temp_cmd);
	}
	return (result);
}

char	*find_cmd_path(char *cmd, char *path)
{
	char	**dirs;
	char	*temp;
	char	*temp_cmd;
	char	*result;

	dirs = ft_split(path, ':');
	free(path);
	temp = get_first_word(cmd);
	if (cmd[0] != '/')
		temp_cmd = ft_strjoin("/", temp);
	else
		temp_cmd = cmd;
	free(temp);
	result = iterate_path(dirs, cmd, temp_cmd);
	free_string_array(dirs);
	return (result);
}
