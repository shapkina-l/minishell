# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/09 11:15:57 by lshapkin          #+#    #+#              #
#    Updated: 2025/03/26 20:41:34 by lshapkin         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC := cc
FLAGS := -g -lreadline -lhistory #-Werror -Wextra -Wall 
NAME := minishell
SRC := tokenization.c parse.c execution/exec.c execution/builtins1.c execution/builtins2.c main.c execution/redirection.c
OBJ := $(SRC:.c=.o)
INCLUDES := libft/libft.a

all: $(NAME)

$(NAME): $(OBJ)
	@make -C libft/ all
	$(CC) $(OBJ) -o $(NAME) $(FLAGS) $(INCLUDES) 

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@
	
clean:
	@make -C libft/ clean
	/bin/rm -f $(OBJ) 

fclean: clean
	/bin/rm -f $(NAME)
	@make -C libft/ fclean

re: fclean all
