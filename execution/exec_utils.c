/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 17:24:02 by lshapkin          #+#    #+#             */
/*   Updated: 2025/05/03 23:11:00 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_error(const char *prefix, const char *msg)
{
	if (prefix)
	{
		write(2, prefix, strlen(prefix));
		write(2, ": ", 2);
	}
	if (msg)
	{
		write(2, msg, strlen(msg));
		write(2, "\n", 1);
	}
}

void	exec_file_check(t_data *data)
{
	struct stat	file_stat;

	if (access(data->args[0], F_OK) == 0)
	{
		if (stat(data->args[0], &file_stat) == 0)
		{
			if (S_ISDIR(file_stat.st_mode))
				print_error(data->args[0], "Is a directory");
			else
				print_error(data->args[0], "Permission denied");
		}
		else
			print_error(data->args[0], "stat failed");
		exit(126);
	}
	else
	{
		print_error(data->args[0], "No such file or directory");
		exit(127);
	}
}

void	close_fd(int fd[2])
{
	if (fd[0] != -1)
		close(fd[0]);
	if (fd[1] != -1)
		close(fd[1]);
}

t_data	*find_last_redirection(t_data *data, int type)
{
	t_data	*left;
	t_data	*right;

	if (!data)
		return (NULL);
	left = find_last_redirection(data->left, type);
	right = find_last_redirection(data->right, type);
	if (right)
		return (right);
	if (left)
		return (left);
	if (data->type == REDIRECTION && data->redirection_type == type)
		return (data);
	return (NULL);
}

t_data	*get_command_node(t_data *data)
{
	if (!data)
		return (NULL);
	if (data->type != REDIRECTION)
		return (data);
	return (get_command_node(data->left));
}
