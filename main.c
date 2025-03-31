/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 15:16:26 by lshapkin          #+#    #+#             */
/*   Updated: 2025/03/29 15:30:05 by apaz-mar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *ft_readline()
{
    char *input;
    char *prompt;
    //printf(ANSI_COLOR_PINK"minishell>"ANSI_COLOR_RESET);
    prompt = "minishell>";
    input = readline(prompt);
    if (input)
        add_history(input);
    return (input);
}

char **dup_envp(char **envp) //copy of the environment to avoid undefined behaviour of non allocated envp
{
    int     i;
    int     len;
    char    **new_env;

    i = 0;
    len = 0;
    while (envp[len])
        len++;
    new_env = malloc(sizeof(char *) * (len + 1));
    if (!new_env)
        return (NULL);
    while (envp[i])
    {
        new_env[i] = ft_strdup(envp[i]);
        i++;
    }
    new_env[i] = NULL;
    return (new_env);
}

void    free_envp(char **envp)
{
    int i;

    i = 0;
    while (envp[i])
        free(envp[i++]);
    free(envp);
}

void    free_exec(t_data *data)
{
    int i;

    if (!data)
        return ;
    if (data->args) //free the args array
    {
        i = 0;
        while (data->args[i])
            free(data->args[i++]);
        free(data->args);
    }
    if (data->full_cmd) //free command string
        free(data->full_cmd);
    if (data->redirection_file) //free redirection file name
        free(data->redirection_file);
    if (data->left) // recursively free left and right branches
        free_exec(data->left);
    if (data->right)
        free_exec(data->right);
    free(data); //free the node itself
}

int main (int argc, char **argv, char *envp[])
{
    //init
    char *input;
    t_data *root;
    char    **my_envp;
    
    (void)argc;
    (void)argv;
    my_envp = dup_envp(envp);
    if (!my_envp)
    {
        printf("Failed to initialize environment.\n");
        return (1);
    }
    while (1) //data->end_flag == 0
    {
        input = ft_readline();
        if (!input)
            break ;
        root = parse_input(input);
        if (!root)
        {
            free(input);
            continue ; // it skips execute and free if root fails
        }
        execute(root, &my_envp);
        free(input);
        free_exec(root);
    }
    free_envp(my_envp);
    return (0);
}
