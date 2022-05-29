# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dirony <dirony@student.21-school.ru>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/02/12 14:50:20 by dirony            #+#    #+#              #
#    Updated: 2022/05/29 15:30:57 by dirony           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRCS = 	main.c						parse_utils.c\
		list_utils.c				error_utils.c\
		exec_utils.c				builtin_echo.c\
		builtin_env.c				builtin_unset.c\
		builtin_exit.c				builtin_cd.c\
		builtin_pwd.c				free_utils.c\
		builtin_export.c			builtin_export_add_utils.c\
		builtin_export_sort_utils.c	builtin_utils.c\
		exec_pipe_utils.c			handler_utils.c\
		lexer_utils.c				lexer_utils2.c\
		lexer_utils3.c				check_syntax_utils.c\
		ast_parse_utils.c			parse_tree_utils.c\
		parse_tree_utils2.c			free_utils2.c\
		new_exec_utils.c			parse_utils2.c\
		pre_parsing_utils.c			pre_parsing_utils2.c\
		heredoc_utils.c				get_next_line.c\
	
LIBDIR =   ./libft/

LIBFT =   libft.a	

OBJS 	=	$(SRCS:%.c=%.o)

DEP	=	$(SRCS:%.c=%.d)

CC = cc

HOMEBREW_PREFIX := $(shell test -n "$$(which brew)" \
					&& brew config | grep HOMEBREW_PREFIX | cut -d' ' -f2)

RM = rm -f

INC_DIR 	= ./includes/

LDLIBS = -lreadline

# RL_INC_DIR	:=	$(HOMEBREW_PREFIX)/opt/readline/include
RL_INC_DIR	:=	$(HOMEBREW_PREFIX)/Cellar/readline/8.1.2/include

# RL_LIB_DIR	:=	$(HOMEBREW_PREFIX)/opt/readline/lib
RL_LIB_DIR	:=	$(HOMEBREW_PREFIX)/Cellar/readline/8.1.2/lib

CPPFLAGS 	= -I$(INC_DIR) -I$(RL_INC_DIR)

LDFLAGS 	= -L$(RL_LIB_DIR)

CFLAGS 		= -Wall -Wextra -Werror -MMD #-g -fsanitize=address

all		:	libmake $(NAME)

libmake :	
			@make -C $(LIBDIR)

$(NAME)	:	$(OBJS) $(LIBDIR)$(LIBFT)
			@cp $(LIBDIR)$(LIBFT) $(LIBFT)
			@$(CC)  $(CFLAGS) $(CPPFLAGS) -g $(LDFLAGS) $(LIBFT) \
			$(LDLIBS) $(OBJS) -o $@  
			@printf "\e[38;5;226m./$@ successfully build\n"

%.o 	: 	%.c
			@$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

-include $(DEP)

test	:  
			cd test
			bash test.sh my
			cd ..
clean	: 		
			@make clean -C $(LIBDIR)
			@$(RM) $(OBJS) $(DEP)
			@printf "\e[38;5;121m.o files deleted\n"

fclean	:	clean
			@make fclean -C $(LIBDIR)
			@$(RM) $(NAME) $(LIBFT) 
			@printf "\e[38;5;121mBinaries deleted\n"

re		:	fclean all

.PHONY	: 	all clean fclean re libmake test
