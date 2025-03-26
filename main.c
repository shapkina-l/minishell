/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 15:16:26 by lshapkin          #+#    #+#             */
/*   Updated: 2025/03/26 20:29:05 by lshapkin         ###   ########.fr       */
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
int main (int argc, char *argv[], char *envp[])
{
    //init
    char *input;
    t_data *root;
    
    while (1) //data->end_flag == 0
    {
        input = ft_readline();
        root = parse_input(input);
        execute(root, envp);
    }
    //free
}