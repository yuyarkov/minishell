# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/02/12 14:50:20 by dirony            #+#    #+#              #
#    Updated: 2022/03/18 20:53:51 by fdarkhaw         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRCS = 	main.c						parse_utils.c\
		list_utils.c				error_utils.c\
		exec_utils.c				builtin_utils.c\
		builtin_env.c				builtin_unset.c\
	
LIBDIR =   ./libft/

LIBFT =   libft.a	

OBJS 	=	$(SRCS:%.c=%.o)

DEP	=	$(SRCS:%.c=%.d)

CC = gcc

HOMEBREW_PREFIX := $(shell test -n "$$(which brew)" \
                        && brew config | grep HOMEBREW_PREFIX | cut -d' ' -f2)

RM = rm -f

INC_DIR 	= ./includes/

LDLIBS = -lreadline

RL_INC_DIR	:=	$(HOMEBREW_PREFIX)/opt/readline/include

RL_LIB_DIR	:=	$(HOMEBREW_PREFIX)/opt/readline/lib

CPPFLAGS 	= -I$(INC_DIR) -I$(RL_INC_DIR)

LDFLAGS 	= -L$(RL_LIB_DIR)

CFLAGS 		= -MMD -Wall -Wextra -Werror -g3

all		:	libmake $(NAME)

libmake :	
			@make -C $(LIBDIR)

$(NAME)	:	$(OBJS) $(LIBDIR)$(LIBFT)
			@cp $(LIBDIR)$(LIBFT) $(LIBFT)
			@$(CC)  $(CFLAGS) $(CPPFLAGS) -g $(LDFLAGS) $(LIBFT) $(LDLIBS) $(OBJS) -o $@  
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