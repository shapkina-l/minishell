/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 01:00:54 by lshapkin          #+#    #+#             */
/*   Updated: 2025/04/10 00:21:51 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_data	*create_pipe_node(t_data *left, t_token *right_token, char **my_envp)
{
	t_data	*pipe_node;
	t_data	*right;

	if (!right_token)
		return (free(left), NULL);
	right = parse_pipe(right_token, my_envp);
	if (!right)
		return (free(left), NULL);
	pipe_node = create_new_node(my_envp);
	if (!pipe_node)
		return (free(left), free(right), NULL);
	pipe_node->type = PIPE;
	pipe_node->left = left;
	pipe_node->right = right;
	return (pipe_node);
}

t_data	*parse_pipe(t_token *token, char **my_envp)
{
	t_data	*left;
	t_data	*pipe_node;
	t_token	*tmp;
	t_token	*right_tokens;

	if (!token)
		return (NULL);
	// Step 1: Find the first pipe token
	tmp = token;
	while (tmp && tmp->type != TOKEN_PIPE)
		tmp = tmp->next;

	if (!tmp || tmp->type != TOKEN_PIPE)
	{
		// no pipe, parse full command + redirections
		left = parse_command(token, my_envp);
		if (!left)
			return (NULL);
		left = parse_redirection(token, left, my_envp);
		if (!left)
			return (NULL);
		return (left);
	}	
	right_tokens = tmp->next; // for freeing the tokens after the cut

	tmp->next = NULL; //cut token list for left side

	// Step 2: Parse left side up to the pipe
	left = parse_command(token, my_envp);
	if (!left)
	{
		free_token_list(right_tokens);
		return (NULL);
	}
	left = parse_redirection(token, left, my_envp);
	if (!left)
	{
		free_token_list(right_tokens);
		return (NULL);
	}

	// Step 3: Create pipe node and recursively handle right
	pipe_node = create_pipe_node(left, right_tokens, my_envp);
	return (pipe_node);
}

void	free_token_list(t_token *head)
{
	t_token	*tmp;

	while (head)
	{
		tmp = head;
		head = head->next;
		if (tmp->value)
			free(tmp->value);
		free(tmp);
	}
}

t_data	*parse_input(char *input, int *exit_status, char **my_envp)
{
	t_token	*token_list;
	t_token	*token;
	t_data	*root;

	if (!input || !*input)
		return (NULL);
	token_list = tokenize(input, exit_status);
	if (!token_list)
		return (NULL);
	token = token_list;
	root = parse_pipe(token, my_envp);
	free_token_list(token_list);
	token_list = NULL;
	return (root);
}
