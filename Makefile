# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/09 11:15:57 by lshapkin          #+#    #+#              #
#    Updated: 2025/03/16 15:16:17 by lshapkin         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC := cc
FLAGS := -g #-Werror -Wextra -Wall 
NAME := minishell
SRC := tmp_main.c execution/exec.c execution/builtins1.c execution/builtins2.c
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
