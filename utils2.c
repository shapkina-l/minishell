/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 21:35:16 by lshapkin          #+#    #+#             */
/*   Updated: 2025/04/02 22:54:39 by lshapkin         ###   ########.fr       */
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
