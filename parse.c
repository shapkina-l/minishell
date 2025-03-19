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
    char buffer[100];
    int i = 0;
    int in_quotes = 0;
    int in_single_quotes = 0;
    int arg_index = 0;

    root = create_new_node(NULL);
    if (!root)
        return (NULL);

    current = root;

    while (*input)
    {
        if (*input == '"')
            in_quotes = !in_quotes;
        else if (*input == '\'')
            in_single_quotes = !in_single_quotes;
        else if (*input == ' ' && !in_quotes && !in_single_quotes) // Split tokens
        {
            if (i > 0)
            {
                buffer[i] = '\0';
                store_token(current, buffer, &arg_index, in_single_quotes);
                i = 0;
            }
        }
        else if (*input == '|')
        {
            if (i > 0)
            {
                buffer[i] = '\0';
                store_token(current, buffer, &arg_index, in_single_quotes);
                i = 0;
            }

            current->pipe = 1;
            current->right = create_new_node(current);
            if (!current->right)
                return (NULL);

            current = current->right;
            arg_index = 0;
        }
        else if (*input == '>' || *input == '<')
        {
            if (i > 0)
            {
                buffer[i] = '\0';
                store_token(current, buffer, &arg_index, in_single_quotes);
                i = 0;
            }

            current->redirect = 1;
            current->right = create_new_node(current);
            if (!current->right)
                return (NULL);

            current = current->right;

            input++; // Skip spaces
            while (*input == ' ')
                input++;

            i = 0;
            while (*input && *input != ' ' && *input != '|' && *input != '<' && *input != '>') // Read the file name into the buffer
            {
                buffer[i++] = *input;
                input++;
            }
            buffer[i] = '\0';

            current->redirect_file = ft_strdup(buffer);
        }
        else
            buffer[i++] = *input;

        input++;
    }

    if (i > 0)
    {
        buffer[i] = '\0';
        store_token(current, buffer, &arg_index, in_single_quotes);
    }

    current->args[arg_index] = NULL;
    return (root);
}

void store_token(t_cmd *current, char *buffer, int *arg_index, int in_single_quotes)
{
    char *expanded_token;

    buffer[ft_strlen(buffer)] = '\0'; // to ensure

    if (buffer[0] == '$' && !in_single_quotes)  //Expand variables only if NOT in single quotes
        expanded_token = expand_variable(buffer);
    else
        expanded_token = ft_strdup(buffer);

    if (!current->cmd) //If the command has not been set, store this token as the command
        current->cmd = expanded_token;
    else
        current->args[(*arg_index)++] = expanded_token; // Otherwise, store it as an argument
}
