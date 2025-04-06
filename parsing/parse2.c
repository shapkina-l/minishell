/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 01:00:54 by lshapkin          #+#    #+#             */
/*   Updated: 2025/04/06 21:37:22 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_data	*create_pipe_node(t_data *left, t_token *token, char **my_envp)
{
	t_data	*pipe_node;
	t_data	*right;

	token = token->next;
	if (!token)
		return (free(left), NULL);
	right = parse_pipe(token, my_envp);
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

	if (!token)
		return (NULL);
	left = parse_command(token, my_envp);
	if (!left)
		return (NULL);
	while (token && token->type != TOKEN_PIPE)
	{
		if (token->type >= TOKEN_REDIRECT_IN && token->type <= TOKEN_APPEND)
		{
			left = parse_redirection(token, left, my_envp);
			if (!left)
				return (NULL);
			token = token->next->next;
		}
		else
			token = token->next;
	}
	if (!token || token->type != TOKEN_PIPE)
		return (left);
	pipe_node = create_pipe_node(left, token, my_envp);
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
	return (root);
}
