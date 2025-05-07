/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 19:27:54 by lshapkin          #+#    #+#             */
/*   Updated: 2025/05/07 21:22:20 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_heredoc_sigint = 0;

void	handle_heredoc_signal(int sig)
{
	(void)sig;
	g_heredoc_sigint = 1;
	write(STDOUT_FILENO, "\n", 1);
	close(STDIN_FILENO);
}

int	process_all_heredocs(t_data *node, int *exit_status)
{
	if (!node)
	{
		return (0);
	}
	if (process_all_heredocs(node->left, exit_status))
		return (1);
	if (node->type == REDIRECTION && node->redirection_type == REDIRECT_HEREDOC)
	{
		if (handle_heredoc(node, exit_status) != 0)
			return (1);
	}
	if (process_all_heredocs(node->right, exit_status))
		return (1);
	return (0);
}

void	process_heredoc_lines(int fd, const char *delimiter,
			int *exit_status)
{
	char	*line;
	char	*expanded;

	while (1)
	{
		line = readline("> ");
		if (!line || g_heredoc_sigint || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		expanded = expand_vars_in_line(line, exit_status);
		free(line);
		if (!expanded)
		{
			close(fd);
			exit(1);
		}
		write(fd, expanded, ft_strlen(expanded));
		free(expanded);
		write(fd, "\n", 1);
	}
}

int	run_heredoc_child(const char *temp_file, const char *delimiter,
	int *exit_status)
{
	int	fd;

	signal(SIGINT, handle_heredoc_signal);
	fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("heredoc temp file");
		exit(1);
	}
	process_heredoc_lines(fd, delimiter, exit_status);
	close(fd);
	free((char *)delimiter);
	free((char *)temp_file);
	if (g_heredoc_sigint)
		exit(130);
	else
		exit(0);
}
