/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 21:39:09 by jg                #+#    #+#             */
/*   Updated: 2022/05/18 21:39:34 by fdarkhaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	lern_size_env(t_env *env)
{
	int	size;

	size = 0;
	while (env)
	{
		if (env->value)
			size++;
		env = env->next;
	}
	return (size);
}

char	**return_env_to_char(t_env *env)
{
	int		size;
	int		index;
	char	**new_envp;

	size = lern_size_env(env);
	new_envp = (char **)malloc(sizeof(char *) * (size + 1));
	if (!new_envp)
		exit(1);
	index = 0;
	while (index < size)
	{
		while (!env->value && env)
			env = env->next;
		new_envp[index] = env_strjoin(env->key, env->value);
		if (!new_envp)
			exit(1);
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

t_env	*create_env(char **envp)
{
	int		iterator;
	t_env	*new_elem;
	t_env	*list;

	new_elem = NULL;
	list = env_create_elem(envp[0]);
	if (!list)
		exit(1);
	iterator = 1;
	while (envp[iterator])
	{
		new_elem = env_create_elem(envp[iterator]);
		if (!new_elem)
			exit(1);
		env_lstadd_back(&list, new_elem);
		iterator++;
	}
	return (list);
}
