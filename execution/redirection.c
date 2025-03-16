/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 12:37:47 by lshapkin          #+#    #+#             */
/*   Updated: 2025/03/16 14:40:43 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void redirect_input(t_data *data)
{
    int fd;

    fd = open(data->redirection_file, O_RDONLY);
    if (fd == -1) 
    {
        perror("Error opening file");
        return;
    }    
    dup2(fd, 1);
    close(fd);
}

void redirect_output(t_data *data)
{
    int fd;

    fd = open(data->redirection_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) 
    {
        perror("Error opening file");
        return;
    }    
    dup2(fd, 0);
    close(fd);
}

void redirect_append(t_data *data)
{
    int fd;

    fd = open(data->redirection_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1) 
    {
        perror("Error opening file");
        return;
    }    
    dup2(fd, 0);
    close(fd);
}

