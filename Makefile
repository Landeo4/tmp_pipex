# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tpotilli <tpotilli@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/10 16:41:59 by tpotilli          #+#    #+#              #
#    Updated: 2024/01/20 12:23:34 by tpotilli         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	Pipex

OBJS_PATH	= objs/

SRCS	=	srcs/main.c\
			srcs/init_struct.c\
			srcs/execute.c\
			srcs/ft_child_process.c\
			srcs/Pipex.c\
			srcs/ft_create_and_verify.c\
			libft/ft_split.c\
			libft/ft_strlen.c\
			libft/ft_strnstr.c\
			libft/ft_substr.c\
			libft/ft_strdup.c\
			libft/ft_strjoin.c\
			libft/ft_strncmp.c\
			libft/ft_is_alpha.c\
			libft/ft_strlcpy.c\

OBJS = $(addprefix $(OBJS_PATH), $(SRCS:.c=.o))

CC		=	gcc

CFLAGS	=	-Wall -Werror -Wextra -g3 -Iincludes

all : ${NAME}

${NAME}:	${OBJS}
			${CC} ${CFLAGS} ${OBJS} -o ${NAME}

clean:
			rm -rf ${OBJS}

fclean:		clean
			rm -rf ${NAME}

re:			fclean all

.PHONY:	all clean fclean re

${OBJS_PATH}%.o: %.c
		@mkdir -p $(@D)
		${CC} ${CFLAGS} -c $< -o $@