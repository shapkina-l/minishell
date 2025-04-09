/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 17:17:56 by apaz-mar          #+#    #+#             */
/*   Updated: 2025/04/09 16:26:40 by lshapkin         ###   ########.fr       */
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

void handle_all_args(t_token *token, t_data *node)
{
    int count_args = 0;
    t_token *tmp;
    int i;

    // First pass: count all command arguments (words that aren't redirection targets)
    tmp = token;
    while (tmp && tmp->type != TOKEN_PIPE)
    {
        if (tmp->type == TOKEN_WORD)
        {
            count_args++;
        }
        else if (tmp->type >= TOKEN_REDIRECT_IN && tmp->type <= TOKEN_APPEND)
        {
            // Skip the redirection target
            if (tmp->next && tmp->next->type == TOKEN_WORD)
                tmp = tmp->next;
        }
        tmp = tmp->next;
    }

    // Allocate arguments array
    node->args = malloc((count_args + 1) * sizeof(char *));
    if (!node->args)
    {
        free(node);
        return;
    }

    // Second pass: fill arguments array
    tmp = token;
    i = 0;
    while (tmp && tmp->type != TOKEN_PIPE && i < count_args)
    {
        if (tmp->type == TOKEN_WORD)
        {
            node->args[i] = ft_strdup(tmp->value);
            if (!node->args[i])
            {
                while (i > 0)
                    free(node->args[--i]);
                free(node->args);
                free(node);
                return;
            }
            i++;
        }
        else if (tmp->type >= TOKEN_REDIRECT_IN && tmp->type <= TOKEN_APPEND)
        {
            // Skip the redirection target
            if (tmp->next && tmp->next->type == TOKEN_WORD)
                tmp = tmp->next;
        }
        tmp = tmp->next;
    }
    node->args[i] = NULL;
}

t_data	*parse_command(t_token *token, char **my_envp)
{
	t_data	*node;
	int		builtin;

	if (!token)
		return (NULL);
	node = create_new_node(my_envp);
	if (!node)
		return (NULL);
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
	handle_all_args(token, node);
	return (node);
}

t_data *parse_redirection(t_token *token, t_data *cmd_node, char **my_envp)
{
    t_data *redir_node;
    t_token *tmp;
    t_data *result;

    result = cmd_node;
    tmp = token;
    
    while (tmp && tmp->type != TOKEN_PIPE)
    {
        if (tmp->type >= TOKEN_REDIRECT_IN && tmp->type <= TOKEN_APPEND)
        {
            if (!tmp->next || tmp->next->type != TOKEN_WORD)
                return (NULL);
            
            redir_node = create_new_node(my_envp);
            if (!redir_node)
                return (NULL);
            
            if (tmp->type == TOKEN_REDIRECT_IN)
                redir_node->redirection_type = REDIRECT_INPUT;
            else if (tmp->type == TOKEN_REDIRECT_OUT)
                redir_node->redirection_type = REDIRECT_OUTPUT;
            else if (tmp->type == TOKEN_APPEND)
                redir_node->redirection_type = REDIRECT_APPEND;
            else if (tmp->type == TOKEN_HEREDOC)
                redir_node->redirection_type = REDIRECT_HEREDOC;
            
            redir_node->redirection_file = ft_strdup(tmp->next->value);
            if (!redir_node->redirection_file)
                return (NULL);
            
            redir_node->type = REDIRECTION;
            redir_node->left = result;
            result = redir_node;
			tmp = tmp->next;
        }
        tmp = tmp->next;
    }
    
    return (result);
}

