/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 18:43:37 by lshapkin          #+#    #+#             */
/*   Updated: 2025/05/06 19:30:43 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	prepare_heredoc(t_data *data, char **temp_file,
	char **delimiter, int *should_expand)
{
	*temp_file = create_heredoc_tempfile();
	if (!*temp_file)
		return (1);
	*should_expand = !is_quoted_delimiter(data->redirection_file);
	*delimiter = ft_strdup(data->redirection_file);
	if (!*delimiter)
	{
		free(*temp_file);
		return (1);
	}
	return (0);
}

int	finalize_heredoc(int status, char *temp_file,
		t_data *data, int *exit_status)
{
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
	{
		unlink(temp_file);
		free(temp_file);
		*exit_status = WEXITSTATUS(status);
		return (1);
	}
	free(data->redirection_file);
	data->redirection_file = temp_file;
	return (0);
}

int	handle_heredoc(t_data *data, int *exit_status)
{
	char	*temp_file;
	char	*delimiter;
	int		should_expand;
	pid_t	pid;
	int		status;

	if (prepare_heredoc(data, &temp_file, &delimiter, &should_expand))
		return (1);
	pid = fork();
	if (pid < 0)
		return (handle_fork_error(temp_file, delimiter));
	if (pid == 0)
		run_heredoc_child(temp_file, delimiter, should_expand, exit_status);
	waitpid(pid, &status, 0);
	free(delimiter);
	return (finalize_heredoc(status, temp_file, data, exit_status));
}

int	redirect_heredoc(t_data *data)
{
	int	fd;

	fd = open(data->redirection_file, O_RDONLY);
	if (fd == -1)
	{
		perror("Error opening heredoc file");
		return (1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

void	cleanup_heredoc_files(t_data *root)
{
	if (!root)
		return ;
	if (root->type == REDIRECTION && root->redirection_type == REDIRECT_HEREDOC
		&& root->redirection_file)
	{
		if (ft_strncmp(root->redirection_file,
				"/tmp/minishell_heredoc_", 22) == 0)
			unlink(root->redirection_file);
	}
	cleanup_heredoc_files(root->left);
	cleanup_heredoc_files(root->right);
}
