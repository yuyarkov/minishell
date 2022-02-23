/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 19:55:35 by dirony            #+#    #+#             */
/*   Updated: 2022/02/23 16:04:15 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_exit_command(char *str)
{
	if (ft_strncmp(str, "exit\0", 5) == 0)
		return (1);
	else
		return (0);
}

int is_builtin_command(char *s)
{
	if (ft_strncmp(s, "exit", 4) == 0)
		return (1);
	if (ft_strncmp(s, "cd", 2) == 0)//как отличать случаи cdk, cdcd и т.д.?
		return (1);
	
	return (0);
}

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
	if (cmd[0] != '/') //похоже на костыль, чтобы не добавлять второй / 
		cmd = ft_strjoin("/", temp);
	//printf("get_cmd_name: %s\n", cmd);
	free(temp);
	while (dirs[i] && !result)
	{
		if (ft_strncmp(dirs[i], cmd, ft_strlen(dirs[i])) != 0)//ещё костыль, чтобы учесть команды, где уже указан полный путь
			temp = ft_strjoin(dirs[i], cmd);
		else
			temp = ft_strdup(cmd);
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
	//printf("результат из find_cmd_path: %s\n", result);
	return (result);
}

char	*get_cmd_path(char *cmd, char **envp)
{
	int		i;
	int		j;
	char	start[5];
	int		result;

//	if (access(cmd, 1 << 0) == 0) //если подали команду уже с путём
//		return (cmd); //но при этом перестаёт работать pwd
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

void	get_info_from_string(char *s, t_info *info)
{
	int	i;
	int	j;
	int	num;

	i = 0;
	num = 0;
	while (s[i] && s[i + 1])
	{
		if (s[i] == ';')
			num++;
		if (ft_strncmp(&s[i], "&&", 2) == 0 || ft_strncmp(&s[i], "||", 2) == 0)
			num++;
		i++;			
	}
	if (num == 0)
		info->num_of_commands = 1;
	else
		info->num_of_commands = num + 1;
	info->limiters = malloc(sizeof(int) * info->num_of_commands);
	if (NULL == info->limiters)
		exit(EXIT_FAILURE);
	i = 0;
	j = 0;
	while (s[i] && s[i + 1])
	{
		if (s[i] == ';')
			info->limiters[j++] = SEMICOLON;
		if (ft_strncmp(&s[i], "&&", 2) == 0)
			info->limiters[j++] = AND_SIGN;
		if (ft_strncmp(&s[i], "||", 2) == 0)
			info->limiters[j++] = OR_SIGN;
		i++;
	}
	if (num == 0)
		info->limiters[0] = SEMICOLON;
}