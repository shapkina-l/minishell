# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lshapkin <lshapkin@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/09 11:15:57 by lshapkin          #+#    #+#              #
#    Updated: 2025/05/21 14:46:55 by lshapkin         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC := cc
CFLAGS := -g -Werror -Wextra -Wall 
LDFLAGS = -lreadline -lhistory
NAME := minishell
SRC := parsing/tokenization.c parsing/tokenization2.c parsing/parse1.c parsing/parse2.c parsing/parse_args.c \
execution/exec.c execution/exec2.c execution/exec_utils.c execution/exec_utils3.c execution/builtins1.c execution/builtins2.c execution/builtins3.c main.c \
execution/redirection.c utils.c utils2.c utils3.c heredoc.c heredoc2.c heredoc3.c execution/exec_utils2.c execution/redirection_utils.c 
OBJ := $(SRC:.c=.o)
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose
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

valgrind-supp:
	@echo "creating valgrind suppression file"
	@echo "{" > readline.supp
	@echo "   Readline" >> readline.supp
	@echo "   Memcheck:Leak" >> readline.supp
	@echo "   match-leak-kinds: reachable" >> readline.supp
	@echo "   ..." >> readline.supp
	@echo "   fun:readline" >> readline.supp
	@echo "}" >> readline.supp
	@echo "{" >> readline.supp
	@echo "   AddHistory" >> readline.supp
	@echo "   Memcheck:Leak" >> readline.supp
	@echo "   match-leak-kinds: reachable" >> readline.supp
	@echo "   ..." >> readline.supp
	@echo "   fun:add_history" >> readline.supp
	@echo "}" >> readline.supp
valgrind: ${NAME} valgrind-supp
	$(VALGRIND) --suppressions=readline.supp ./$(NAME)
memcheck: ${NAME} valgrind-supp
	$(VALGRIND) --suppressions=readline.supp --log-file=valgrind_output.txt ./$(NAME)

re: fclean all

.PHONY: all clean fclean re

