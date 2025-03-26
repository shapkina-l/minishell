/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 17:17:56 by apaz-mar          #+#    #+#             */
/*   Updated: 2025/03/26 22:06:19 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


//changes - t_cmd -> t_data
//norminette friendly
//command parsing finds full path to the command in the env variable
//everything splitted into little functions
//added tree srtucture, before it was a linked list

char	*make_cmd(char **cmd_path, char *argv)
{
	char	*tmp;
	char	*cmd;

	if (access(argv, 0) == 0)
		return (argv);
	while (*cmd_path)
	{
		tmp = ft_strjoin(*cmd_path, "/");
		cmd = ft_strjoin(tmp, argv);
		free (tmp);
		if (access(cmd, 0) == 0)
			return (cmd);
		free (cmd);
		cmd_path++;
	}
	return (NULL);
}

t_data *parse_command(t_token *token) 
{
    char *path;
    char **cmd_path;
    char *full_cmd;
    t_data *node;
    
    if (!token) 
        return (NULL);
    
    node = malloc(sizeof(t_data));
    if (!node) 
        return (NULL);
    //check if it is a builtin
    node->type = EXECUTION;
    node->args = NULL;
    node->redirection_type = 0;
    node->redirection_file = NULL;
    node->left = NULL;
    node->right = NULL;
    path = getenv("PATH");
    if (!path)
        return(NULL);
    cmd_path = ft_split(path, ':');
    full_cmd = make_cmd(cmd_path, token->value);
    if (!full_cmd)
        return (free(cmd_path), NULL);
    node->full_cmd = full_cmd;

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
        // Store command as args[0]
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
    
        free(cmd_path); // Clean up cmd_path after use
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
    t_data *pipe_node;
    t_data *right;

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
            token = token->next->next; // Skip redirection and file
        }
        else
            token = token->next;
    }
    if (!token || token->type != TOKEN_PIPE)
        return (left);
    
    token = token->next;
    right = parse_command(token);
    if (!right) 
        return NULL;
    // Create a pipe node
    pipe_node = malloc(sizeof(t_data));
    if (!pipe_node)
        return (free(left), free(right), NULL);
    pipe_node->type = PIPE;
    pipe_node->left = left;
    pipe_node->right = right;
    pipe_node->args = NULL;
    pipe_node->full_cmd = NULL;
    pipe_node->redirection_type = 0;
    pipe_node->redirection_file = NULL;
    return (pipe_node);
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
    return (root);
}

// to do - redirection and parse

// t_data    *parse_input(char *input)
// {
//     t_data   *root = NULL; //norm
//     t_data   *current = NULL;
//     t_token **token_list = tokenize(input);
//     t_token *token = *token_list;
//     //int arg_index = 0;

//     if (!token)
//         return (NULL);
        
//     root = create_new_node(NULL);
//     if (!root)
//         return (NULL);

//     current = root;

//     while (token)
//     {
//         if (token->type == TOKEN_WORD)
//             store_token(current, token->value, &arg_index);
//         else if (token->type == TOKEN_PIPE)
//         {
//             current->pipe = 1;
//             current->right = create_new_node(current);
//             if (!current->right)
//                 return (NULL);
//             current = current->right;
//             arg_index = 0;
//         }
//         else if (token->type == TOKEN_REDIRECT_IN || token->type == TOKEN_REDIRECT_OUT
//             || token->type == TOKEN_HEREDOC || token->type == TOKEN_APPEND)
//         {
//             current->redirect = 1;
//             current->redirection_type = token->type;

//             token = token->next;
//             if (!token || token->type != TOKEN_WORD)
//                 return (NULL);

//             current->redirect_file = ft_strdup(token->value);
//         }
//         token = token->next;
//     }
//     current->args[arg_index] = NULL;
//     return (root);
// }