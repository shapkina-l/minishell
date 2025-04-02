/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 01:00:54 by lshapkin          #+#    #+#             */
/*   Updated: 2025/04/03 01:05:15 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_data *parse_pipe(t_token *token)
{
	t_data	*left;

	if (!token)
		return (NULL);
	left = parse_command(token);
	if (!left)
		return (NULL);
	while (token && token->type != TOKEN_PIPE)
	{
		if (token->type >= TOKEN_REDIRECT_IN && token->type <= TOKEN_APPEND)
		{
			left = parse_redirection(token, left);
			if (!left)
				return (NULL);
			token = token->next->next; // Skip redirection operator and file
		}
		else
			token = token->next;
	}
	// If no pipe, return the command (with possible redirection)
	if (!token || token->type != TOKEN_PIPE)
		return (left);
	// Found a pipe, advance and parse the right side recursively
	token = token->next;
	if (!token)
		return (free(left), NULL); // Pipe with no right command is invalid

	t_data *right = parse_pipe(token); // Recursive call for multiple pipes
	if (!right)
		return (free(left), NULL);

	// Create pipe node
	
	t_data *pipe_node = create_new_node();
	if (!pipe_node)
		return (free(left), free(right), NULL);
	pipe_node->type = PIPE;
	pipe_node->left = left;
	pipe_node->right = right;

	return (pipe_node);
}

void    free_token_list(t_token *head)
{
	t_token *tmp;

	while (head)
	{
		tmp = head;
		head = head->next;
		if (tmp->value)
			free(tmp->value);
		free(tmp);
	}
}

t_data	*parse_input(char *input)
{
	t_token	*token_list;
	t_token	*token;
	t_data	*root;

	if (!input || !*input)
		return (NULL);
	token_list = tokenize(input);
	if (!token_list)
		return (NULL);
	token = token_list;
	root = parse_pipe(token);
	free_token_list(token_list);
	return (root);
}