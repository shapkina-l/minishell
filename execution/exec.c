/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:43:30 by lshapkin          #+#    #+#             */
/*   Updated: 2025/05/14 16:25:04 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exec_child(t_data *data)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (data->full_cmd == NULL)
	{
		if (data->args[0][0] == '/' ||
			(data->args[0][0] == '.' && (data->args[0][1] == '/' ||
			(data->args[0][1] == '.' && data->args[0][2] == '/'))))
			exec_file_check(data);
		else
		{
			print_error(data->args[0], "command not found");
			exit(127);
		}
	}
	exec_child_access_check(data);
	execve(data->full_cmd, data->args, data->my_envp);
	if (errno == ENOEXEC)
		exit(0);
	perror("execve");
	exit(EXIT_FAILURE);
}

int	exec(t_data *data)
{
	int	status;
	int	pid;

	if (!data->args || !data->args[0] || data->args[0][0] == '\0')
		return (0);
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		exec_child(data);
		execve(data->full_cmd, data->args, data->my_envp);
		if (errno == ENOEXEC)
			exit(0);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else
		return (1);
}

int	builtin(t_data *data, int *exit_status)
{
	if (data->builtin_type == BUILTIN_ECHO)
		return (ft_echo(data));
	else if (data->builtin_type == BUILTIN_CD)
		return (ft_cd(data));
	else if (data->builtin_type == BUILTIN_PWD)
		return (ft_pwd());
	else if (data->builtin_type == BUILTIN_EXPORT)
		return (ft_export(data));
	else if (data->builtin_type == BUILTIN_UNSET)
		return (ft_unset(data));
	else if (data->builtin_type == BUILTIN_ENV)
		return (ft_env(data->my_envp));
	else if (data->builtin_type == BUILTIN_EXIT)
		return (ft_exit(data, exit_status));
	return (1);
}

// int	execute_redirection(t_data *data, int *ret, int *exit_status)
// {
// 	t_data	*command_node;

// 	command_node = get_command_node(data);
// 	if (!command_node)
// 		return (1);
// 	if (check_all_files(data))
// 		return (perror(data->redirection_file), 1);
// 	apply_redirections(data);
// 	if (command_node->type == EXECUTION || command_node->type == BUILTIN)
// 		*ret = execute(command_node, exit_status);
// 	else
// 		return (1);
// 	return (0);
// }

int	execute_redirection(t_data *data, int *ret, int *exit_status)
{
	t_data	*command_node;

	command_node = get_command_node(data);
	if (!command_node)
		return (1);
	if (check_all_files(data))
		return (perror(data->redirection_file), 1);
	apply_redirections(data);
	if (command_node->type == EXECUTION)
		*ret = exec(command_node);
	else if (command_node->type == BUILTIN) //move to a separate function
	{
		pid_t pid;
		int   status;

		pid = fork();
		if (pid < 0)
			return (perror("fork"), 1);
		if (pid == 0)
		{
			signal(SIGINT,  SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			exit(builtin(command_node, exit_status));
		}
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			*ret = WEXITSTATUS(status);
		else
			*ret = 1;
	}
	else
		return (1);
	return (0);
}

int	execute(t_data *data, int *exit_status)
{
	int		ret;

	ret = 1;
	if (!data)
		return (1);
	if (data->type == EXECUTION)
		ret = exec(data);
	else if (data->type == PIPE)
		ret = pipes(data, exit_status);
	else if (data->type == REDIRECTION)
	{
		if (execute_redirection(data, &ret, exit_status) == 1)
			return (1);
	}
	else if (data->type == BUILTIN)
		ret = builtin(data, exit_status);
	//reset_redirections(data->original_stdin, data->original_stdout);
	*exit_status = ret;
	return (ret);
}
