# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fdarkhaw <fdarkhaw@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/10/05 20:08:26 by dirony            #+#    #+#              #
#    Updated: 2022/05/21 18:12:42 by fdarkhaw         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

HEADER = libft.h

SRC = 		ft_isalpha.c		ft_calloc.c\
			ft_isdigit.c		ft_strdup.c\
			ft_isalnum.c		ft_substr.c\
			ft_isalnum.c		ft_strjoin.c\
			ft_isascii.c		ft_strtrim.c\
			ft_isprint.c		ft_split.c\
			ft_strlen.c			ft_itoa.c\
			ft_memset.c			ft_strmapi.c\
			ft_bzero.c			ft_striteri.c\
			ft_memcpy.c			ft_putchar_fd.c\
			ft_memmove.c		ft_putstr_fd.c\
			ft_strlcpy.c		ft_putendl_fd.c\
			ft_strlcat.c		ft_putnbr_fd.c\
			ft_toupper.c		ft_tolower.c\
			ft_strchr.c			ft_strrchr.c\
			ft_strncmp.c		ft_memchr.c\
			ft_memcmp.c			ft_strnstr.c\
			ft_atoi.c			ft_strcmp.c\
			ft_lstnew_bonus.c\
			ft_lstadd_front_bonus.c		ft_lstiter_bonus.c\
			ft_lstadd_back_bonus.c		ft_lstmap_bonus.c\
			ft_lstlast_bonus.c			ft_lstdelone_bonus.c\
			ft_lstclear_bonus.c			ft_lstsize_bonus.c\

SRC_BONUS = ft_lstnew_bonus.c			ft_lstsize_bonus.c\
			ft_lstadd_front_bonus.c		ft_lstiter_bonus.c\
			ft_lstadd_back_bonus.c		ft_lstmap_bonus.c\
			ft_lstlast_bonus.c			ft_lstdelone_bonus.c\
			ft_lstclear_bonus.c

OBJ = ${SRC:.c=.o}

OBJ_BONUS = ${SRC_BONUS:.c=.o}

CFLAGS = -Wall -Werror -Wextra

COMPILE = gcc $(CFLAGS) -c

LIB = ar rc $(NAME)

RANLIB = ranlib $(NAME)

REMOVE = rm -f

.PHONY: all bonus clean fclean re

all: $(NAME)

$(NAME): $(OBJ) $(HEADER)
	@$(COMPILE) $(SRC)
	@$(LIB) $(OBJ)
	@$(RANLIB)

bonus:
	@make OBJ="$(OBJ) $(OBJ_BONUS)" all

clean:
	@$(REMOVE) $(OBJ) $(OBJ_BONUS)

fclean: clean
	@$(REMOVE) $(NAME)

re: fclean all
