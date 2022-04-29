#include "minishell.h"

void	print_commands_list(t_list *cmd)
{
	t_list	*iterator;

	iterator = cmd;
	while (iterator)
	{
		printf("print_commands_list. parsed command: %s\n", iterator->cmd);
		printf("print_commands_list. previous command: %p\n", iterator->previous);
		iterator = iterator->next;
	}	
}


void	printf_env(t_env *env)
{
	while (env)
	{
		printf("k - %s === v - %s\n", env->key, env->value);
		env = env->next;
	}
}

void	printf_char_pointer(char **str)
{
	int	i = 0;
	while (str[i])
		printf("our envp = %s\n", str[i++]);
}

void	print_tokens(t_info *info)
{
	int	i;
	t_token	*t;

	t = info->tokens;
	printf("Num of tokens: %d\n", info->num_of_tokens);
	i = 0;
	while (i <= info->num_of_tokens)
	{
		if (t[i].left && t[i].right)
			printf("level: %d\tgroup_id: %d\tvalue: %s\tleft: %s, %d\tright: %s, %d\n", 
			t[i].level, t[i].group_id, t[i].value, t[i].left->value, t[i].left->type,
					 t[i].right->value, t[i].right->type);
		else
			printf("level: %d\tgroup_id: %d\tvalue: %s,\ttype: %d\n", 
			t[i].level, t[i].group_id, t[i].value, t[i].type);
		i++;
	}
}

// void	print_tokens(t_info *info)// Не удаляй плз, мне для отладки нажна эта версия
// {
// 	int	i;

// 	printf("Num of tokens: %d\n", info->num_of_tokens);
// 	i = 0;
// 	while (i < info->num_of_tokens)
// 	{
// 		printf("token %d\ttype: %d,\tvalue: %s\n", i, info->tokens[i].type, info->tokens[i].value);
// 		i++;
// 	}
// }
