/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 19:55:35 by dirony            #+#    #+#             */
/*   Updated: 2022/05/25 21:21:49 by fdarkhaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_exit_command(char *str, t_info *info)
{
	if (!str)
		return (0);
	if (ft_strncmp(str, "exit\0", 5) == 0)
	{
		clear_info(info);
		return (1);
	}
	else
		return (0);
}

int	is_builtin_command(char *s)
{
	if (!s)
		return (0);
	if (ft_strncmp(s, "exit", 4) == 0)
		return (1);
	if ((ft_strncmp(s, "cd ", 3) == 0) || (ft_strncmp(s, "cd\0", 3) == 0))
		return (1);
	if (ft_strncmp(s, "echo ", 5) == 0)
		return (1);
	if (ft_strncmp(s, "pwd", 3) == 0)
		return (1);
	if (ft_strncmp(s, "unset\0", 6) == 0)
		return (1);
	if (ft_strncmp(s, "export\0", 7) == 0)
		return (1);
	return (0);
}

char	*get_first_word(char *cmd)
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
	char	*temp_cmd;
	int		i;
	char	*result;

	dirs = ft_split(path, ':');
	i = 0;
	result = NULL;
	temp = get_first_word(cmd);
	if (cmd[0] != '/')
		temp_cmd = ft_strjoin("/", temp);
	else
		temp_cmd = cmd;
	free(temp);
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
	free_string_array(dirs);
	if (cmd == temp_cmd)
		free(temp_cmd);
	else
	{
		free(cmd);
		free(temp_cmd);
	}
	return (result);
}

char	*get_cmd_path(char *input_cmd, char **envp, t_info *info)
{
	int		i;
	int		j;
	char	*cmd;
	char	start[5];
	int		result;

	(void) info;
	cmd = ft_strdup(input_cmd);
	if (cmd == '\0')
		return (cmd);
	if (access(cmd, 1 << 0) == 0)
		return (cmd);
	if (is_builtin_command(cmd))
		return (cmd);
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

// char	**split_commands_by_limiters(char *str, t_info *info)
// {
// 	char	**result;
// 	char	*temp;
// 	int		length;
// 	int		index;
// 	int		i;

// 	result = malloc(sizeof(char *) * (info->num_of_commands + 1));
// 	if (NULL == result)
// 		exit(EXIT_FAILURE);
// 	result[0] = ft_strtrim(ft_substr(str, 0, info->limiters[0].index), SPACES);//подумать над освобождением строк после trim и substr
// 	i = 1;
// 	while (i < info->num_of_commands)
// 	{
// 		index = info->limiters[i - 1].index + (info->limiters[i - 1].sign % 2 + 1);
// 		length = info->limiters[i].index - index;
// 		temp = ft_substr(str, index, length);
// 		result[i] = ft_strtrim(temp, SPACES);
// 		free(temp);
// 		i++;
// 	}
// 	result[i] = NULL;
// 	return (result);
// }
