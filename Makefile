# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lshapkin <lshapkin@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/09 11:15:57 by lshapkin          #+#    #+#              #
#    Updated: 2025/04/15 17:28:48 by lshapkin         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC := cc
CFLAGS := -g -Werror -Wextra -Wall 
LDFLAGS = -lreadline -lhistory
NAME := minishell
SRC := parsing/tokenization.c parsing/parse1.c parsing/parse2.c parsing/parse_args.c execution/exec.c execution/exec_utils.c execution/builtins1.c execution/builtins2.c main.c execution/redirection.c utils.c utils2.c
OBJ := $(SRC:.c=.o)
INCLUDES := libft/libft.a

all: $(NAME)

$(NAME): $(OBJ)
	@make -C libft/ all
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS) $(INCLUDES) 

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
	
clean:
	@make -C libft/ clean
	/bin/rm -f $(OBJ) 

fclean: clean
	/bin/rm -f $(NAME)
	@make -C libft/ fclean

re: fclean all

.PHONY: all clean fclean re

