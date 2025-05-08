/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 21:35:16 by lshapkin          #+#    #+#             */
/*   Updated: 2025/05/04 21:36:00 by lshapkin         ###   ########.fr       */
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
	value = "$";
	while (**input && (ft_isalnum(**input) || **input == '_'))
	{
		var_name[var_len++] = **input;
		(*input)++;
	}
	if (var_len != 0)
	{
		var_name[var_len] = '\0';
		value = getenv(var_name);
		if (!value)
			value = "";
	}
	while (*value)
		buffer[buf_index++] = *value++;
	return (buf_index);
}

void	my_shell_handler(int signum)
{
	(void)signum;
	write(STDOUT_FILENO, "\n", 1);
	if (g_shell_state & 1)
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r');
}

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;
	size_t	copy_size;

	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	new_ptr = malloc(new_size);
	if (!new_ptr)
		return (NULL);
	if (ptr)
	{
		if (old_size < new_size)
			copy_size = old_size;
		else
			copy_size = new_size;
		ft_memcpy(new_ptr, ptr, copy_size);
		free(ptr);
	}
	return (new_ptr);
}
