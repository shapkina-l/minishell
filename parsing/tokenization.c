/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:17:19 by lshapkin          #+#    #+#             */
/*   Updated: 2025/04/23 19:57:33 by lshapkin         ###   ########.fr       */
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

void	tokenize_word_loop(char *buffer, char **input, int *exit_status)
{
	int		buf_index;
	char	*status_str;
	char	quote_char = 0;

	buf_index = 0;
	while (**input)
	{
		if (!quote_char && ft_strchr(" \t|<>", **input)) // there can be an operator in the middle of a word, then break and the loop will handle the operator
			break ;
		if (**input == '$' && *(*input + 1) == '?' && quote_char != '\'')
		{
			(*input) += 2;
			status_str = ft_itoa(*exit_status);
			ft_strlcpy(&buffer[buf_index], status_str, 1024 - buf_index);
			buf_index += ft_strlen(status_str);
			free(status_str);
			continue ;
		}
		if ((**input == '\'' || **input == '"') && quote_char == 0)
		{
			quote_char = **input;
			(*input)++;
			continue ;
		}
		if (**input == quote_char)
		{
			quote_char = 0;
			(*input)++;
			continue ;
		}
		if (**input == '$' && quote_char != '\'')
		{
			buf_index = handle_env_var(input, buffer, buf_index);
			continue ;
		}
		buffer[buf_index++] = **input;
		(*input)++;
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
	if (buffer[0] == '\0') // Handle case where buffer is empty (e.g., standalone "")
	{
		free(buffer);
		return (NULL); // Or create an empty token, depending on your needs
	}
	dup_value = ft_strdup(buffer);
	free(buffer);
	if (!dup_value)
		return (NULL); // prevent leak if strdup fails
	token = create_token(TOKEN_WORD, dup_value);
	if (!token)
		free(dup_value);
	return (token);
}

t_token	*tokenize(char *input, int *exit_status)
{
	t_token	*head;
	t_token	*current;
	t_token	*new_token;

	head = NULL;
	current = NULL;
	while (*input)
	{
		skip_whitespaces(&input);
		if (!*input)
			break ;
		new_token = NULL;
		if (ft_strchr("|<>", *input))
			new_token = tokenize_operator(&input);
		else
			new_token = tokenize_word(&input, exit_status);
		if (!new_token)
			continue ;
		//💥 NEW check: if new_token has empty value, free it and skip
		if (new_token->value == NULL || new_token->value[0] == '\0')
		{
			free(new_token->value);
			free(new_token);
			continue;
		}
		if (!head)
			head = new_token;
		else
			current->next = new_token;
		current = new_token;
	}
	return (head);
}
