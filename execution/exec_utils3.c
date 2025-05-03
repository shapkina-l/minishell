/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 20:46:17 by lshapkin          #+#    #+#             */
/*   Updated: 2025/05/03 20:55:38 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exec_child_access_check(t_data *data)
{
	if (access(data->full_cmd, X_OK) == -1)
	{
		if (access(data->full_cmd, F_OK) == 0)
			exit(0);
		else
		{
			perror(data->full_cmd);
			exit(126);
		}
	}
}

int	has_output_redirection(t_data *data)
{
	if (!data)
		return (0);
	if (data->type == REDIRECTION
		&& (data->redirection_type == REDIRECT_OUTPUT
			|| data->redirection_type == REDIRECT_APPEND))
		return (1);
	return (has_output_redirection(data->left)
		|| has_output_redirection(data->right));
}

int	has_input_redirection(t_data *data)
{
	if (!data)
		return (0);
	if (data->type == REDIRECTION
		&& (data->redirection_type == REDIRECT_INPUT
			|| data->redirection_type == REDIRECT_HEREDOC))
		return (1);
	return (has_input_redirection(data->left)
		|| has_input_redirection(data->right));
}

void	clean_exit_child(t_data *branch, int ret)
{
	free_exec(branch);
	rl_clear_history();
	exit(ret);
}
