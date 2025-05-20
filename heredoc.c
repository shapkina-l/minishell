/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 19:27:54 by lshapkin          #+#    #+#             */
/*   Updated: 2025/05/20 01:10:01 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//1 << 1: This is a bitwise left shift operation.
//The 1 in binary is 0001 (assuming we're working with 4 bits for simplicity).
//The operation 1 << 1 shifts the bits of 1 to the left by 1 position. 
//The result is 0010, which is 2 in decimal.
//So, (1 << 1) equals 2 (binary 0010).
//g_shell_state |= (1 << 1): Now, you're performing a bitwise OR assignment.
//The value of g_shell_state is bitwise OR'd with the result of 1 << 1, 
//which is 2 (binary 0010).

void	handle_heredoc_signal(int sig)
{
	(void)sig;
	g_shell_state = g_shell_state | (1 << 1);
	write(STDOUT_FILENO, "\n", 1);
	close(STDIN_FILENO);
}

int	process_all_heredocs(t_data *node, int *exit_status, char **my_envp)
{
	if (!node)
	{
		return (0);
	}
	if (process_all_heredocs(node->left, exit_status, my_envp))
		return (1);
	if (node->type == REDIRECTION && node->redirection_type == REDIRECT_HEREDOC)
	{
		if (handle_heredoc(node, exit_status, my_envp) != 0)
			return (1);
	}
	if (process_all_heredocs(node->right, exit_status, my_envp))
		return (1);
	return (0);
}

void	process_heredoc_lines(int fd, const char *delimiter,
			int *exit_status, char **my_envp)
{
	char	*line;
	char	*expanded;

	while (1)
	{
		line = readline("> ");
		if (!line || (g_shell_state & (1 << 1))
			|| ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		expanded = expand_vars_in_line(line, *exit_status, my_envp);
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
	int *exit_status, char **my_envp)
{
	int	fd;

	signal(SIGINT, handle_heredoc_signal);
	fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("heredoc temp file");
		exit(1);
	}
	process_heredoc_lines(fd, delimiter, exit_status, my_envp);
	close(fd);
	free((char *)delimiter);
	free((char *)temp_file);
	if (g_shell_state & (1 << 1))
		exit(130);
	else
		exit(0);
}
