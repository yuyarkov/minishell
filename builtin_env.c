/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 21:39:09 by jg                #+#    #+#             */
/*   Updated: 2022/03/29 20:39:49 by fdarkhaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	search_path(char **envp)
// {
// 	int	iterator;

// 	iterator = 0;
// 	while (envp[iterator])
// 	{
// 		if (!ft_strncmp(envp[iterator], "PATH=", 5))//нужно искать в PATH user/bin
// 			return (1); //если есть PATH
// 		iterator++;
// 	}
// 	return (0); //если нет PATH
// }

// int	execute_env_command(t_list *cmd, char **envp)
// {
// 	int	result;

// 	(void) cmd;
// 	if (search_path(envp))
// 	{
// 		while (*envp)
// 			ft_putendl_fd(*(envp++), 1);
// 		result = 0;
// 	}
// 	else
// 	{
// 		ft_putendl_fd("minishell: env: No such file or directory", 2);
// 		result = 127;
// 	}
// 	return (result);
// }

char	*env_strjoin(char const *s1, char const *s2)
{
	char	*str;
	int		i;
	int		j;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	str = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 2));
	if (str == NULL)
		return (NULL);
	i = 0;
	while (s1[i] != '\0')
	{
		str[i] = s1[i];
		i++;
	}
	str[i++] = '=';
	j = 0;
	while (s2[j] != '\0')
		str[i++] = s2[j++];
	str[i] = '\0';
	return (str);
}

char	**return_env_to_char(t_env *env)
{
	int		size;
	int		index;
	char	**new_envp;
	t_env	*tmp;

	size = 0;
	tmp = env;
	while (tmp)
	{
		if (tmp->value)
			size++;
		tmp = tmp->next;
	}
	new_envp = (char **)malloc(sizeof(char *) * (size + 1));// может вернуть NULL
	index = 0;
	while (index < size)
	{
		while (!env->value && env)
			env = env->next;
		new_envp[index] = env_strjoin(env->key, env->value);// может вернуть NULL
		env = env->next;
		index++;
	}
	new_envp[index] = NULL;
	return (new_envp);
}

void	env_lstadd_back(t_env **list, t_env *new_elem)
{
	t_env	*iterator;

	iterator = *list;
	if (*list)
	{
		while (iterator->next)
			iterator = iterator->next;
		iterator->next = new_elem;
	}
	else
		*list = new_elem;
}

t_env	*env_create_elem(char *str)
{
	t_env	*new_elem;
	char	**result;

	new_elem = malloc(sizeof(t_env));
	if (NULL == new_elem)
		return (NULL);
	new_elem->next = NULL;
	if (ft_strchr(str, '='))
	{	
		result = ft_split(str, '=');
		new_elem->key = ft_substr(result[0], 0, ft_strlen(result[0]));// может вернуть NULL
		if (result[1])
			new_elem->value = ft_substr(result[1], 0, ft_strlen(result[1]));// может вернуть NULL
		free_str_pointer(result);
	}
	else
	{
		new_elem->key = ft_substr(str, 0, ft_strlen(str));// может вернуть NULL
		new_elem->value = NULL;
	}
	return (new_elem);
}

t_env	*create_env(char **envp)
{
	int		iterator;
	t_env	*new_elem;
	t_env	*list;

	new_elem = NULL;
	list = env_create_elem(envp[0]);// может вернуть NULL
	iterator = 1;
	while (envp[iterator])
	{
		new_elem = env_create_elem(envp[iterator]);// может вернуть NULL
		env_lstadd_back(&list, new_elem);
		iterator++;
	}
	return (list);
}
