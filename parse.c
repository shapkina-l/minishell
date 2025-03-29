/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 17:17:56 by apaz-mar          #+#    #+#             */
/*   Updated: 2025/03/27 23:12:58 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_data *parse_command(t_token *token) 
{
    char *path;
    char **cmd_path;
    char *full_cmd;
    t_data *node;
    
    if (!token) 
        return (NULL);
    
    node = create_new_node();
    if (!node) 
        return (NULL);
    int builtin = builtin_check(token->value);
    if (builtin != -1)
    {
        node->type = BUILTIN;
        node->builtin_type = builtin;
    }
    else
        node->type = EXECUTION;
    if (node->type == EXECUTION)
    {
        path = getenv("PATH");
        if (!path)
            return(NULL);
        cmd_path = ft_split(path, ':');
        full_cmd = make_cmd(cmd_path, token->value);
        if (!full_cmd)
           return (free(cmd_path), NULL);
        node->full_cmd = full_cmd;
    }
    int count_args = 1;
    t_token *tmp = token->next;
    int i = 0;
    while (tmp && tmp->type == TOKEN_WORD)
    {
        tmp = tmp->next;
        count_args++;
    }
    node->args = malloc((count_args + 1) * sizeof(char *));
    if (!node->args)
        return (free(full_cmd), free(cmd_path), free(node), NULL);
        node->args[0] = ft_strdup(token->value);
        if (!node->args[0])
        {
            free(node->args);
            free(full_cmd);
            free(cmd_path);
            free(node);
            return (NULL);
        }
        // Store remaining arguments
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
                free(full_cmd);
                free(cmd_path);
                free(node);
                return (NULL);
            }
            i++;
            token = token->next;
        }
    node->args[i] = NULL;
    if (node->type == EXECUTION)
        free(cmd_path);
    return (node);
}

t_data	*parse_redirection(t_token *token, t_data *cmd_node)
{
    if (!token || !cmd_node)
        return (NULL);
    if (token->type == TOKEN_REDIRECT_IN)
        cmd_node->redirection_type = REDIRECT_INPUT;
    else if (token->type == TOKEN_REDIRECT_OUT)
        cmd_node->redirection_type = REDIRECT_OUTPUT;
    else if (token->type == TOKEN_APPEND)
        cmd_node->redirection_type = REDIRECT_APPEND;
    else if (token->type == TOKEN_HEREDOC)
        cmd_node->redirection_type = REDIRECT_HEREDOC;
    else
        return (cmd_node); // No redirection, return as is
    token = token->next;
    if (!token || token->type != TOKEN_WORD)
        return (NULL); // Missing file after redirection
    cmd_node->redirection_file = ft_strdup(token->value);
    if (!cmd_node->redirection_file)
        return (NULL);
    return (cmd_node);
}

t_data *parse_pipe(t_token *token)
{
    t_data *left;
    t_token *start;

    if (!token)
        return (NULL);

    start = token;
    left = parse_command(token);
    if (!left)
        return (NULL);

    // Advance token to next potential pipe or redirection
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
    // Free token list here if you have a free_tokens function
    free_token_list(token_list);
    return (root);
}