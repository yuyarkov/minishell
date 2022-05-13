/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 19:55:35 by dirony            #+#    #+#             */
/*   Updated: 2022/05/13 21:02:37 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_exit_command(char *str)
{
	if (!str)
		return (0);
	if (ft_strncmp(str, "exit\0", 5) == 0)
		return (1);
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
						//Артём - можно поставить пробел или \0 после cd.
						//Юра - как отличать случаи cdk, cdcd и т.д.?
		return (1);
	if (ft_strncmp(s, "echo ", 5) == 0)
		return (1);
	if (ft_strncmp(s, "pwd", 3) == 0)
		return (1);
	// if (ft_strncmp(s, "env\0", 4) == 0)
	// 	return (1);
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
	if (cmd[0] != '/') //похоже на костыль, чтобы не добавлять второй / 
		temp_cmd = ft_strjoin("/", temp);
	else
		temp_cmd = cmd;
	//printf("get_cmd_name: %s\n", cmd);
	free(temp);
	while (dirs[i] && !result)
	{
		if (ft_strncmp(dirs[i], temp_cmd, ft_strlen(dirs[i])) != 0)//ещё костыль, чтобы учесть команды, где уже указан полный путь
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
	//printf("результат из find_cmd_path: %s\n", result);
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
	//printf("cmd before access: %s\n", cmd);
	if (access(cmd, 1 << 0) == 0) //если подали команду уже с путём
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

void	parse_limiters(char *s, t_info *info)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s[i] && s[i + 1])
	{
		if (s[i] == ';')
		{
			info->limiters[j].sign = SEMICOLON;
			info->limiters[j++].index = i;
		}
		if (s[i] == '|')
		{
			info->limiters[j].sign = PIPE;
			info->limiters[j++].index = i;
		}	
		if (ft_strncmp(&s[i], "&&", 2) == 0)
		{
			info->limiters[j].sign = AND_SIGN;
			info->limiters[j++].index = i;
		}
		if (ft_strncmp(&s[i], "||", 2) == 0)
		{
			info->limiters[j].sign = OR_SIGN;
			info->limiters[j++].index = i;
		}
		i++;
	}
	//printf("inside parse limiters\n");
	info->limiters[j].sign = SEMICOLON;
	info->limiters[j].index = ft_strlen(s);//для последнего команды ставлю параметры виртуального разделителя
}

void	get_info_from_string(char *str, t_info *info)
{
	int		i;
	int		num;
	char	*s;

	s = ft_strtrim(str, SPACES);
	//printf("after trim, \"%s\"\n", s);
	i = 0;
	num = 0;
	while (s[i] && s[i + 1])
	{
		if (s[i] == ';' || s[i] == '|')
			num++;
		if (ft_strncmp(&s[i], "&&", 2) == 0 || ft_strncmp(&s[i], "||", 2) == 0)
			num++;
		i++;
	}
	if (num == 0)
		info->num_of_commands = 1;
	else
		info->num_of_commands = num + 1;
	info->limiters = malloc(sizeof(t_limiter) * info->num_of_commands);
	if (NULL == info->limiters)// при таком выходе не очищаются структуры
		exit(EXIT_FAILURE);
	parse_limiters(s, info);
	if (num == 0)
	{
		info->limiters[0].sign = SEMICOLON; //костыль, когда команда одна, заполняю как будто ; в конце
		info->limiters[0].index = ft_strlen(s);
	//printf("num of commands: %d\n", info->num_of_commands);
	}
	free(s);
}



char	**split_commands_by_limiters(char *str, t_info *info)
{
	char	**result;
	char	*temp;
	int		length;
	int		index;
	int		i;

	result = malloc(sizeof(char *) * (info->num_of_commands + 1));
	if (NULL == result)
		exit(EXIT_FAILURE);
	result[0] = ft_strtrim(ft_substr(str, 0, info->limiters[0].index), SPACES);//подумать над освобождением строк после trim и substr
	//printf("result[0]: %s\n", result[0]);
	i = 1;
	while (i < info->num_of_commands)
	{
		index = info->limiters[i - 1].index + (info->limiters[i - 1].sign % 2 + 1);
		length = info->limiters[i].index - index;
		temp = ft_substr(str, index, length);
		result[i] = ft_strtrim(temp, SPACES);
		free(temp);
		//printf("result[%d]: %s\n", i, result[i]);
		i++;
	}
	result[i] = NULL;
	return (result);
}

// void	parse_commands(char *str, t_info *info, char **envp)
// {
// 	char	**commands;
// 	t_list	*result;

// 	if (info->num_of_commands > 1) // здесь надо будет выделить в отдельную функцию поиск и разделение по ;, && и т.д.
// 		commands = split_commands_by_limiters(str, info);
// 	else
// 	{
// 		commands = malloc(sizeof(char *) * 2);
// 		commands[0] = ft_strtrim(str, SPACES); //утечка
// 		commands[1] = NULL;//и помечаю пустым последний элемент массива строк
// 	}
// 					//printf("commands before add: %s, num_of_commands: %d\n", commands[0], info->num_of_commands);
// 	result = add_cmd_to_list(info, commands, envp);
// 					//printf("before return from parse commands, cmd: %s\n", result->cmd);
// 	free_string_array(commands);
// 	// return (result);
// 	info->commands = result;
// }
