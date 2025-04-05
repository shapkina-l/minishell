/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 17:17:56 by apaz-mar          #+#    #+#             */
/*   Updated: 2025/04/05 16:38:34 by apaz-mar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	node_full_cmd(t_token *token, t_data *node) // adapted to handle PATH="" correctly
{
	char	*path = NULL;
	char	**cmd_path = NULL;
	char	*full_cmd = NULL;
	int		i;

	i = 0;
	path = getenv("PATH");
	if (!path || !*path)
	{
		printf("Path is empty string\n");
		return ;
	}
	cmd_path = ft_split(path, ':');
	full_cmd = make_cmd(cmd_path, token->value);
	if (cmd_path)
	{
		while (cmd_path[i])
		{
			free(cmd_path[i]);
			i++;
		}
		free(cmd_path);
	}
	if (!full_cmd)
	{
		node->full_cmd = NULL;
		return ;
	}
	node->full_cmd = full_cmd;
}

int	count_arguments(t_token *token, t_data *node)
{
	int		count_args;
	t_token	*tmp;

	count_args = 1;
	tmp = token->next;
	while (tmp && tmp->type == TOKEN_WORD)
	{
		tmp = tmp->next;
		count_args++;
	}
	node->args = malloc((count_args + 1) * sizeof(char *));
	if (!node->args)
	{
		free(node);
		return (0);
	}
	node->args[0] = ft_strdup(token->value);
	if (!node->args[0])
	{
		free(node->args);
		free(node);
		return (0);
	}
	return (count_args);
}

void	handle_args(t_token *token, t_data	*node)
{
	int		count_args;
	int		i;

	count_args = count_arguments(token, node);
	i = 1;
	token = token->next;
	while (token && token->type == TOKEN_WORD && i < count_args)
	{
		node->args[i] = ft_strdup(token->value);
		if (!node->args[i])
		{
			while (i > 0)
				free(node->args[--i]);
			free(node->args);
			free(node);
			return ;
		}
		i++;
		token = token->next;
	}
	node->args[i] = NULL;
}

t_data	*parse_command(t_token *token)
{
	t_data	*node;
	int		builtin;

	if (!token)
		return (NULL);
	node = create_new_node();
	if (!node)
		return (NULL);
	//printf("Checking if [%s] is a builtin\n", token->value);
	builtin = builtin_check(token->value);
	if (builtin != -1)
	{
		node->type = BUILTIN;
		node->builtin_type = builtin;
	}
	else
		node->type = EXECUTION;
	if (node->type == EXECUTION)
		node_full_cmd(token, node);
	handle_args(token, node);
	return (node);
}

t_data	*parse_redirection(t_token *token, t_data *cmd_node)
{
	t_data	*redir_node;

	while (token && (token->type >= TOKEN_REDIRECT_IN && token->type <= TOKEN_APPEND))
	{
		if (!token->next || token->next->type != TOKEN_WORD)
			return (NULL);
		redir_node = create_new_node();
		if (!redir_node)
			return (NULL);
		if (token->type == TOKEN_REDIRECT_IN)
			redir_node->redirection_type = REDIRECT_INPUT;
		else if (token->type == TOKEN_REDIRECT_OUT)
			redir_node->redirection_type = REDIRECT_OUTPUT;
		else if (token->type == TOKEN_APPEND)
			redir_node->redirection_type = REDIRECT_APPEND;
		else if (token->type == TOKEN_HEREDOC)
			redir_node->redirection_type = REDIRECT_HEREDOC;
		redir_node->redirection_file = ft_strdup(token->next->value);
		if (!redir_node->redirection_file)
			return (NULL);
		redir_node->type = REDIRECTION;
		redir_node->left = cmd_node;
		cmd_node = redir_node;
		token = token->next->next;
	}
	return (cmd_node);
}

