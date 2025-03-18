/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:17:19 by lshapkin          #+#    #+#             */
/*   Updated: 2025/03/18 17:18:50 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void skip_whitespaces(char **input)
{
    while (**input && (**input == ' ' || **input == '\t'))
        (*input)++;        
}

t_token *create_token (t_token_type type, char *value)
{
    t_token *token;
    
    token = malloc(sizeof(t_token));
    if (!token)
        return (NULL);
    token->type = type;
    token->value = value;
    token->next = NULL;
    return (token);
}

t_token *tokenize_operator(char **input)
{
    if (**input == '|')
    {
        (*input)++;
        return (create_token(TOKEN_PIPE, ft_strdup("|")));
    }
    if (**input == '<')
    {
        if(*(*input + 1) == '<')
        {
            (*input) += 2;
            return (create_token(TOKEN_HEREDOC, ft_strdup("<<")));
        }
        (*input)++;
        return (create_token(TOKEN_REDIRECT_IN, ft_strdup("<")));
    }
    if (**input == '>')
    {
        if(*(*input + 1) == '>')
        {
            (*input) += 2;
            return (create_token(TOKEN_APPEND, ft_strdup(">>")));
        }
        (*input)++;
        return (create_token(TOKEN_REDIRECT_OUT, ft_strdup(">")));
    }
    return (NULL);
}

t_token *tokenize_word(char **input)
{
    int len = 0;
    char *start = *input;
    while (**input && !ft_strchr(" |<>", **input))
    {
        (*input)++;
        len++;
    }
    return (create_token(TOKEN_WORD, strndup(start, len))); //add ft_strndup to libft
}

t_token *tokenize(char *input)
{
    t_token *head;
    t_token *current;
    t_token *new_token;

    head = NULL;
    current = NULL;
    while (*input)
    {
        skip_whitespaces(&input);
        
        if (!*input)
            break;
            
        new_token = NULL;
        if (ft_strchr("|<>", *input))
            new_token = tokenize_operator(&input);
        else
            new_token = tokenize_word(&input);
        if (!new_token)
            continue;
        if (!head)
            head = new_token;
        else
            current->next = new_token;
        current = new_token;
    }
    return (head);
}

void print_tokens(t_token *tokens)
{
    while (tokens)
    {
        printf("Token Type: %d, Value: %s\n", tokens->type, tokens->value);
        tokens = tokens->next;
    }
}

int main()
{
    char input[] = "echo hello | grep world && ls -l > output.txt";
    t_token *tokens = tokenize(input);
    print_tokens(tokens);
    return 0;
}