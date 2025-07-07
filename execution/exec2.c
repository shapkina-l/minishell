/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 22:37:41 by lshapkin          #+#    #+#             */
/*   Updated: 2025/05/21 17:35:38 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exec_pipe(int fd[2], int var, t_data *exec, int *exit_status)
{
	int	ret;

	if (var == 1)
	{
		if (fd[1] != -1)
			dup2(fd[1], STDOUT_FILENO);
		close_fd(fd);
		ret = execute(exec, exit_status);
		exit(ret);
	}
	else if (var == 2)
	{
		if (fd[0] != -1)
			dup2(fd[0], STDIN_FILENO);
		close_fd(fd);
		ret = execute(exec, exit_status);
		exit(ret);
	}
}

void	pipes_left_child(t_pipes_utils *utils, t_data *data,
	int fd[2], int *exit_status)
{
	if (utils->pid1 < 0)
		perror("fork");
	if (utils->pid1 == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		signal(SIGPIPE, SIG_IGN);
		if (!has_output_redirection(data->left))
			dup2(fd[1], STDOUT_FILENO);
		close_fd(fd);
		if (data->left->type == BUILTIN)
			exit(builtin(data->left, exit_status));
		else
			exit(execute(data->left, exit_status));
	}
}

void	pipes_right_child(t_pipes_utils *utils, t_data *data,
	int fd[2], int *exit_status)
{
	if (utils->pid2 < 0)
		perror("fork");
	if (utils->pid2 == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		signal(SIGPIPE, SIG_IGN);
		if (!has_input_redirection(data->right))
			dup2(fd[0], STDIN_FILENO);
		close_fd(fd);
		if (data->right->type == BUILTIN)
			exit(builtin(data->right, exit_status));
		else
			exit(execute(data->right, exit_status));
	}
}

void	set_pipes_utils(t_pipes_utils	*utils)
{
	utils->pid1 = -1;
	utils->pid2 = -1;
	utils->status1 = 0;
	utils->status2 = 0;
}

int	pipes(t_data *data, int *exit_status)
{
	t_pipes_utils	utils;

	set_pipes_utils(&utils);
	if (pipe(utils.fd) == -1)
		return (perror("pipe"), 1);
	utils.pid1 = fork();
	pipes_left_child(&utils, data, utils.fd, exit_status);
	utils.pid2 = fork();
	pipes_right_child(&utils, data, utils.fd, exit_status);
	close_fd(utils.fd);
	if (utils.pid1 != -1)
		waitpid(utils.pid1, &utils.status1, 0);
	if (utils.pid2 != -1)
		waitpid(utils.pid2, &utils.status2, 0);
	if (utils.pid2 != -1 && WIFEXITED(utils.status2))
		return (WEXITSTATUS(utils.status2));
	if (utils.pid1 != -1 && WIFEXITED(utils.status1))
		return (WEXITSTATUS(utils.status1));
	return (1);
}
