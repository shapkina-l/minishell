/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:17:19 by lshapkin          #+#    #+#             */
/*   Updated: 2025/05/07 23:39:36 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*create_token(t_token_type type, char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = value;
	token->next = NULL;
	return (token);
}

t_token	*tokenize_operator(char **input)
{
	if (**input == '|')
		return ((*input)++, create_token(TOKEN_PIPE, ft_strdup("|")));
	if (**input == '<')
	{
		if (*(*input + 1) == '<')
		{
			(*input) += 2;
			return (create_token(TOKEN_HEREDOC, ft_strdup("<<")));
		}
		(*input)++;
		return (create_token(TOKEN_REDIRECT_IN, ft_strdup("<")));
	}
	if (**input == '>')
	{
		if (*(*input + 1) == '>')
		{
			(*input) += 2;
			return (create_token(TOKEN_APPEND, ft_strdup(">>")));
		}
		(*input)++;
		return (create_token(TOKEN_REDIRECT_OUT, ft_strdup(">")));
	}
	return (NULL);
}

// struct because of maximum 4 arguments
int	process_word_char(t_process_word_data *data, char *buffer, 
	char **input, int buf_index)
{
	char	*status_str;

	if (**input == '$' && *(*input + 1) == '?' && *data->quote_char != '\'')
	{
		(*input) += 2;
		status_str = ft_itoa(*data->exit_status);
		ft_strlcpy(buffer + buf_index, status_str, 1024 - buf_index);
		buf_index += ft_strlen(status_str);
		return (free(status_str), buf_index);
	}
	if ((**input == '\'' || **input == '"') && *data->quote_char == 0)
	{
		*data->quote_char = **input;
		return ((*input)++, buf_index);
	}
	if (**input == *data->quote_char)
	{
		*data->quote_char = 0;
		return ((*input)++, buf_index);
	}
	if (**input == '$' && *data->quote_char != '\'')
		return (handle_env_var(input, buffer, buf_index));
	buffer[buf_index++] = **input;
	return ((*input)++, buf_index);
}

void	tokenize_word_loop(char *buffer, char **input, int *exit_status)
{
	int					buf_index;
	char				quote_char;
	t_process_word_data	data;

	buf_index = 0;
	quote_char = 0;
	data.exit_status = exit_status;
	data.quote_char = &quote_char;
	while (**input)
	{
		if (!quote_char && ft_strchr(" \t|<>", **input))
			break ;
		buf_index = process_word_char(&data, buffer, input, buf_index);
	}
	buffer[buf_index] = '\0';
}

t_token	*tokenize_word(char **input, int *exit_status)
{
	char	*buffer;
	t_token	*token;
	char	*dup_value;

	buffer = malloc(1024);
	if (!buffer)
		return (NULL);
	tokenize_word_loop(buffer, input, exit_status);
	if (buffer[0] == '\0')
	{
		free(buffer);
		return (NULL);
	}
	dup_value = ft_strdup(buffer);
	free(buffer);
	if (!dup_value)
		return (NULL);
	token = create_token(TOKEN_WORD, dup_value);
	if (!token)
		free(dup_value);
	return (token);
}
