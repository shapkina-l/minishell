/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 20:54:07 by lshapkin          #+#    #+#             */
/*   Updated: 2025/05/03 20:54:57 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_all_files(t_data *data)
{
	int	fd;

	if (!data)
		return (0);
	if (data->left && check_all_files(data->left))
		return (1);
	if (data->right && check_all_files(data->right))
		return (1);
	if (data->type == REDIRECTION && data->redirection_type != REDIRECT_HEREDOC)
	{
		fd = -1;
		if (data->redirection_type == REDIRECT_INPUT)
			fd = open(data->redirection_file, O_RDONLY);
		else if (data->redirection_type == REDIRECT_OUTPUT)
			fd = open(data->redirection_file, O_WRONLY | O_CREAT
					| O_TRUNC, 0644);
		else if (data->redirection_type == REDIRECT_APPEND)
			fd = open(data->redirection_file, O_WRONLY | O_CREAT
					| O_APPEND, 0644);
		if (fd == -1)
			return (1);
		close(fd);
	}
	return (0);
}

void	apply_redirection(t_data *redir, int *exit_status)
{
	if (redir->redirection_type == REDIRECT_INPUT)
		redirect_input(redir);
	else if (redir->redirection_type == REDIRECT_OUTPUT)
		redirect_output(redir);
	else if (redir->redirection_type == REDIRECT_APPEND)
		redirect_append(redir);
	else if (redir->redirection_type == REDIRECT_HEREDOC)
		redirect_heredoc(redir, exit_status);
}

void	apply_redirections(t_data *data, int *exit_status)
{
	if (!data)
		return ;
	if (data->left)
		apply_redirections(data->left, exit_status);
	if (data->type == REDIRECTION)
		apply_redirection(data, exit_status);
	if (data->right)
		apply_redirections(data->right, exit_status);
}
