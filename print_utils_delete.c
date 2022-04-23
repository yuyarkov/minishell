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

	printf("Num of tokens: %d\n", info->num_of_tokens);
	i = 0;
	while (i < info->num_of_tokens)
	{
		printf("level: %d\tgroup_id: %d\tvalue: %s\n", 
			info->tokens[i].level, info->tokens[i].group_id, info->tokens[i].value);
		i++;
	}
}
