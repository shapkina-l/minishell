/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:17:19 by lshapkin          #+#    #+#             */
/*   Updated: 2025/03/19 14:25:31 by lshapkin         ###   ########.fr       */
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

int handle_env_var(char **input, char *buffer, int buf_index)
{
    (*input)++;  // Skip '$'
    char var_name[100];  // Assume max length 100
    int var_len = 0;

    // Extract the variable name
    while (**input && (ft_isalnum(**input) || **input == '_')) 
    {
        var_name[var_len++] = **input;
        (*input)++;
    }
    var_name[var_len] = '\0';

    // Get the variable's value
    char *value = getenv(var_name);
    if (!value) value = "";  // Replace with empty string if undefined

    // Copy expanded value to buffer
    while (*value)
        buffer[buf_index++] = *value++;
    return (buf_index);
}

t_token *tokenize_word(char **input) //norm
{
    char *start = *input;
    char *buffer = malloc(1024);  // Buffer for token storage
    int buf_index = 0;
    char quote_char = 0;  // Store quote type (' or ")
    
    while (**input && (!strchr(" |<>&()", **input) || quote_char)) 
    {
        // Handle opening quotes
        if ((**input == '\'' || **input == '"') && quote_char == 0) 
        {
            quote_char = **input;  // Store whether it's ' or "
            (*input)++;  // Skip opening quote
            continue;  // Continue reading inside the quote
        }
        // Handle closing quote
        if (**input == quote_char) 
        {
            (*input)++;  // Skip closing quote
            break;  // End of quoted content
        }
        // Handle variable expansion inside double quotes
        if (**input == '$' && quote_char != '\'') 
        {  // Only expand in double quotes or unquoted
            buf_index = handle_env_var(input, buffer, buf_index);
            continue;  // Don't add the raw $VAR name to buffer
        }
        // Copy character to buffer
        buffer[buf_index++] = **input;
        (*input)++;
    }
    buffer[buf_index] = '\0';  
    
    t_token *token = create_token(TOKEN_WORD, strdup(buffer));  // Create token from buffer
    free(buffer);  // Free buffer after use
    return token;
}

t_token *tokenize(char *input) //handle $HOME ..
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

//temporary for testing
void print_tokens(t_token *tokens)
{
    while (tokens)
    {
        printf("Token Type: %d, Value: %s\n", tokens->type, tokens->value);
        tokens = tokens->next;
    }
}
//temporary for testing
int main()
{
    char input[] = "echo \"123 $HOME\" | grep word";
    t_token *tokens = tokenize(input);
    print_tokens(tokens);
    return 0;
}