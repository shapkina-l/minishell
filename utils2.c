/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 21:35:16 by lshapkin          #+#    #+#             */
/*   Updated: 2025/04/02 23:02:10 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	skip_whitespaces(char **input)
{
	while (**input && (**input == ' ' || **input == '\t'))
		(*input)++;
}

int	handle_env_var(char **input, char *buffer, int buf_index)
{
	char	var_name[100];
	int		var_len;
	char	*value;

	(*input)++;
	var_len = 0;
	while (**input && (ft_isalnum(**input) || **input == '_'))
	{
		var_name[var_len++] = **input;
		(*input)++;
	}
	var_name[var_len] = '\0';
	value = getenv(var_name);
	if (!value)
		value = "";
	while (*value)
		buffer[buf_index++] = *value++;
	return (buf_index);
}

// void	my_shell_handler(int signum)
// {
// 	(void)signum;           //int signum as argument necessary because of the 
//                          //arguments that signal expect
// 	write(1, "\n", 1);      // Move to a new line
// 	rl_replace_line("", 0); // Clear the current input line
// 	rl_on_new_line();       // Prepare a new prompt
// 	rl_redisplay();         // Redisplay the prompt
// }

void	my_shell_handler(int signum)
{
	(void)signum;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}
