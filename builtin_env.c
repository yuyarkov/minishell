/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jg <jg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 21:39:09 by jg                #+#    #+#             */
/*   Updated: 2022/03/17 17:48:28 by jg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_env_command(t_list *cmd, char **envp)
{
	(void) cmd;
	while (*envp)
		ft_putendl_fd(*(envp++), 1);
	return (0);
}

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
	char	**new_env;
	t_env	*tmp;

	size = 0;
	tmp = env;
	while (tmp)
	{
		tmp = tmp->next;
		size++;
	}
	new_env = (char **)malloc(sizeof(char *) * (size + 1));
	index = 0;
	while (index < size)
	{
		new_env[index] = env_strjoin(env->key, env->value);
		env = env->next;
		index++;
	}
	new_env[index] = NULL;
	return (new_env);
}

void	free_str_pointer(char **str)
{
	int	iterator;

	iterator = 0;
	while (str[iterator])
	{
		free(str[iterator]);
		iterator++;
	}
	free(str);
}

// void	ft_lstclear(t_list **list, void (*del)(void *))
// {
// 	t_list	*temp;

// 	while (*list)
// 	{
// 		temp = (*list)->next;
// 		ft_lstdelone(*list, del);
// 		*list = temp;
// 	}
// 	*list = NULL;
// }

// void	ft_lstadd_front(t_list **list, t_list *new_elem)
// {
// 	if (*list)
// 	{
// 		new_elem->next = *list;
// 		*list = new_elem;
// 	}
// 	else
// 		*list = new_elem;
// }

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
	result = ft_split(str, '=');
	new_elem->key = ft_substr(result[0], 0, ft_strlen(result[0]));
	new_elem->value = ft_substr(result[1], 0, ft_strlen(result[1]));
	free_str_pointer(result);
	return (new_elem);
}

t_env	*create_env(char **envp)
{
	int		iterator;
	t_env	*new_elem;
	t_env	*list;

	new_elem = NULL;
	list = env_create_elem(envp[0]);
	iterator = 1;
	while (envp[iterator])
	{
		new_elem = env_create_elem(envp[iterator]);
		env_lstadd_back(&list, new_elem);
		iterator++;
	}
	return (list);
}

void	clear_env(t_env *list)
{
	if (list)
	{
		clear_env(list->next);
		free(list);
		list = NULL;
	}
}

void	lstiter_env(t_env *list, void (*f)(void *))
{
	while (list)
	{
		f(list->key);
		f(list->value);
		list = list->next;
	}
}
