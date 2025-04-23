/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 12:37:47 by lshapkin          #+#    #+#             */
/*   Updated: 2025/04/23 20:17:57 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	redirect_input(t_data *data)
{
	int	fd;

	fd = open(data->redirection_file, O_RDONLY);
	if (fd == -1)
	{
		perror("Error opening file");
		return (1);
	}
	dup2(fd, 0);
	close(fd);
	return (0);
}

int	redirect_output(t_data *data)
{
	int	fd;

	fd = open(data->redirection_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("Error opening file");
		return (1);
	}
	dup2(fd, 1);
	close(fd);
	return (0);
}

int	redirect_append(t_data *data)
{
	int	fd;

	fd = open(data->redirection_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror("Error opening file");
		return (1);
	}
	dup2(fd, 1);
	close(fd);
	return (0);
}

void	reset_redirections(int original_stdin, int original_stdout)
{
	if (original_stdin >= 0)
	{
		dup2(original_stdin, STDIN_FILENO);
		close(original_stdin);
	}

	if (original_stdout >= 0)
	{
		dup2(original_stdout, STDOUT_FILENO);
		close(original_stdout);
	}
}
