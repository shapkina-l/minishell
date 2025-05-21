/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 20:54:07 by lshapkin          #+#    #+#             */
/*   Updated: 2025/05/21 15:05:39 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_all_files_p2(t_data *data)
{
	int	fd;

	fd = -1;
	if (data->redirection_type == REDIRECT_INPUT)
		fd = open(data->redirection_file, O_RDONLY);
	else if (data->redirection_type == REDIRECT_OUTPUT)
		fd = open(data->redirection_file, O_WRONLY | O_CREAT
				| O_TRUNC, 0644);
	else if (data->redirection_type == REDIRECT_APPEND)
		fd = open(data->redirection_file, O_WRONLY | O_CREAT
				| O_APPEND, 0644);
	return (fd);
}

char	*check_all_files(t_data *data)
{
	int		fd;
	char	*broken_file;

	if (!data)
		return (0);
	if (data->left)
	{
		broken_file = check_all_files(data->left);
		if (broken_file != NULL)
			return (broken_file);
	}
	if (data->right)
	{
		broken_file = check_all_files(data->right);
		if (broken_file != NULL)
			return (broken_file);
	}
	if (data->type == REDIRECTION && data->redirection_type != REDIRECT_HEREDOC)
	{
		fd = check_all_files_p2(data);
		if (fd == -1)
			return (data->redirection_file);
		close(fd);
	}
	return (NULL);
}

void	apply_redirection(t_data *redir)
{
	if (redir->redirection_type == REDIRECT_INPUT)
		redirect_input(redir);
	else if (redir->redirection_type == REDIRECT_OUTPUT)
		redirect_output(redir);
	else if (redir->redirection_type == REDIRECT_APPEND)
		redirect_append(redir);
	else if (redir->redirection_type == REDIRECT_HEREDOC)
		redirect_heredoc(redir);
}

void	apply_redirections(t_data *data)
{
	if (!data)
		return ;
	if (data->left)
		apply_redirections(data->left);
	if (data->type == REDIRECTION)
		apply_redirection(data);
	if (data->right)
		apply_redirections(data->right);
}
