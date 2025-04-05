/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:43:30 by lshapkin          #+#    #+#             */
/*   Updated: 2025/04/05 16:39:09 by apaz-mar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exec(t_data *data, char ***envp)
{
	int	status;
	int	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL); // Ctrl + C should terminate the child normally
		signal(SIGQUIT, SIG_DFL); // Ctrl + \ should work normally
		execve(data->full_cmd, data->args, *envp);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	waitpid(pid, &status, 0);
	return (WIFEXITED(status) ? WEXITSTATUS(status) : 1);
//	free_exec(data); //? --> freed in main
}

int	builtin(t_data *data, char ***envp)
{
	if (data->builtin_type == BUILTIN_ECHO)
		return (ft_echo(data));
	else if (data->builtin_type == BUILTIN_CD)
		return (ft_cd(data));
	else if (data->builtin_type == BUILTIN_PWD)
		return (ft_pwd());
	else if (data->builtin_type == BUILTIN_EXPORT)
		return (ft_export(data, envp));
	else if (data->builtin_type == BUILTIN_UNSET)
		return (ft_unset(data, envp));
	else if (data->builtin_type == BUILTIN_ENV)
		return (ft_env(*envp));
	else if (data->builtin_type == BUILTIN_EXIT)
		return (ft_exit());
	return (1);
}

void	close_fd(int fd[2])
{
	if (fd[0] != -1)
		close(fd[0]);
	if (fd[1] != -1)
		close(fd[1]);
}

void	exec_pipe(int fd[2], int var, t_data *exec, char ***envp)
{
	if (var == 1)
	{
		dup2(fd[1], STDOUT_FILENO);
		close_fd(fd);
		execute(exec, envp);
		exit(EXIT_FAILURE);
	}
	else if (var == 2)
	{
		dup2(fd[0], STDIN_FILENO);
		close_fd(fd);
		execute(exec, envp);
		exit(EXIT_FAILURE);
	}
}

int	pipes(t_data *data, char ***envp)
{
	int	fd[2];
	int	pid1;
	int	pid2;
	int	status1;
	int	status2;

	if (pipe(fd) == -1)
		perror("pipe");
	pid1 = fork();
	if (pid1 < 0)
		perror("fork");
	if (pid1 == 0)
		exec_pipe(fd, 1, data->left, envp);
	pid2 = fork();
	if (pid2 == 0)
		exec_pipe(fd, 2, data->right, envp);
	close_fd(fd);
	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	if (WIFEXITED(status2))
		return (WIFEXITED(status2));
	return (1);
}

int	redirection(t_data *data, char ***envp)
{
	if (data->redirection_type == REDIRECT_INPUT)
		redirect_input(data);
	else if (data->redirection_type == REDIRECT_OUTPUT)
		redirect_output(data);
	//else if (data->redirection_type == REDIRECT_HEREDOC)
		// to do
	else if (data->redirection_type == REDIRECT_APPEND)
		redirect_append(data);
	return (execute(data->left, envp));
}

int	execute(t_data *data, char ***envp)
{
	int	ret;

	ret = 1;
	if (!data)
		return (0);
	if (data->type == EXECUTION)
		ret = exec(data, envp);
	else if (data->type == PIPE)
		ret = pipes(data, envp);
	else if (data->type == REDIRECTION)
		ret = redirection(data, envp);
	else if (data->type == BUILTIN)
		ret = builtin(data, envp);
	reset_redirections(data->original_stdin, data->original_stdout);
	return (ret);
}

