/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 19:02:19 by dirony            #+#    #+#             */
/*   Updated: 2022/02/12 17:33:56 by dirony           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	print_prompt_string(void)
{
	int	i;
	int	pipes_num;

	pipes_num = 1;
	i = 0;
	while (i < pipes_num - 1)
	{
		ft_putstr_fd("pipe ", 1);
		i++;
	}
	ft_putstr_fd("minishell$> ", 1);
}

void handler(int num)
{
	printf("поймал сигнал: %d\n", num);
}

int	main(int argc, char **argv)
{
	(void) argc;
	(void) argv;
	char *s;
	char *temp;
	
	signal(SIGINT, handler);//вот тут-то я ловлю сигнал ctrl+C, ctrl+D и ctrl+/
	s = malloc(1000);
	using_history();    /* initialize history */
	temp = NULL;
	while (ft_strncmp(temp, "exit\0", 5) != 0)
	{
		print_prompt_string();
		temp = readline(s);
		add_history(temp);
		printf("%s\n", temp);
	}

	    /* get the state of your history list (offset, length, size) */
    HISTORY_STATE *myhist = history_get_history_state();

    /* retrieve the history list */
    HIST_ENTRY **mylist = history_list();

    printf ("\nsession history:\n");
    for (int i = 0; i < myhist->length; i++) { /* output history list */
        printf ("%d: %8s  %s\n", i, mylist[i]->line, mylist[i]->data);
        free_history_entry (mylist[i]);     /* free allocated entries */
    }
    putchar ('\n');

    free (myhist);  /* free HIST_ENTRY list */
    free (mylist);  /* free HISTORY_STATE   */

	  printf("\nlastline:\n");
  // That's the line you were looking for, I think.
  // int where_history(void) returns the current (that is: last) line
  // HIST_ENTRY *history_get (int) returns the entry data set
  HIST_ENTRY *entry = history_get(where_history());

  printf("%s\n", entry->line);
  free(s);
}