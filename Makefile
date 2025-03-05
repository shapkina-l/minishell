# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lshapkin <lshapkin@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/09 11:15:57 by lshapkin          #+#    #+#              #
#    Updated: 2025/03/05 17:24:27 by lshapkin         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC := cc
FLAGS := -Werror -Wextra -Wall -g 
NAME := minishell
SRC := main.c execution/exec.c
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
