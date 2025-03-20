/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apaz-mar <apaz-mar@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 17:17:56 by apaz-mar          #+#    #+#             */
/*   Updated: 2025/03/05 17:17:58 by apaz-mar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd    *parse_input(char *input)
{
    t_cmd   *root = NULL;
    t_cmd   *current = NULL;
    t_token *token_list = tokenize(input);
    t_token *token = token_list;
    int arg_index = 0;

    if (!token)
        return (NULL);

    root = create_new_node(NULL);
    if (!root)
        return (NULL);

    current = root;

    while (token)
    {
        if (token->type == TOKEN_WORD)
            store_token(current, token->value, &arg_index, 0);
        else if (token->type == TOKEN_PIPE)
        {
            current->pipe = 1;
            current->right = create_new_node(current);
            if (!current->right)
                return (NULL);
            current = current->right;
            arg_index = 0;
        }
        else if (token->type == TOKEN_REDIRECT_IN || token->type == TOKEN_REDIRECT_OUT
            || token->type == TOKEN_HEREDOC || token->type == TOKEN_APPEND)
        {
            current->redirect = 1;
            current->redirection_type = token->type;

            token = token->next;
            if (!token || token->type != TOKEN_WORD)
                return (NULL);

            current->redirect_file = ft_strdup(token->value);
        }
        token = token->next;
    }
    current->args[arg_index] = NULL;
    return (root);
}

void store_token(t_cmd *current, char *value, int *arg_index, int in_single_quotes)
{
    char *expanded_token;

    if (buffer[0] == '$' && !in_single_quotes)  //Expand variables only if NOT in single quotes
        expanded_token = expand_variable(buffer);
    else
        expanded_token = ft_strdup(buffer);

    if (!current->cmd) //If the command has not been set, store this token as the command
        current->cmd = expanded_token;
    else
        current->args[(*arg_index)++] = expanded_token; // Otherwise, store it as an argument
}
