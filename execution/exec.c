/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:43:30 by lshapkin          #+#    #+#             */
/*   Updated: 2025/03/29 16:31:25 by apaz-mar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exec(t_data *data, char **envp)
{
	int pid = fork();
	if (pid < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
	if (pid == 0)
	{
        execve(data->full_cmd, data->args, envp);
        perror("execve");
        exit(EXIT_FAILURE);
    }
	waitpid(pid, NULL, 0);
//	free_exec(data); //? --> freed in main
}

void	builtin(t_data *data, char ***envp)
{
	if (data->builtin_type == BUILTIN_ECHO)
		ft_echo(data);
	else if (data->builtin_type == BUILTIN_CD)
		ft_cd(data);
	else if (data->builtin_type == BUILTIN_PWD)
		ft_pwd();
	else if (data->builtin_type == BUILTIN_EXPORT)
		ft_export(data, envp);
	else if (data->builtin_type == BUILTIN_UNSET)
		ft_unset(data, envp);
	else if (data->builtin_type == BUILTIN_ENV)
		ft_env(*envp);
	else if (data->builtin_type == BUILTIN_EXIT)
		ft_exit();
}

void	pipes(t_data *data, char ***envp)
{
	int	fd[2];
	int	pid1;
	int	pid2;

	if (pipe(fd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	pid1 = fork();
	if (pid1 < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid1 == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		execute(data->left, envp);
		exit(EXIT_FAILURE);
	}
	pid2 = fork();
	if (pid2 == 0)
	{
		dup2(fd[0], STDIN_FILENO);
		close(fd[1]);
		close(fd[0]);
		execute(data->right, envp);
		exit(EXIT_FAILURE);
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}

void	redirection(t_data *data)
{
	if (data->redirection_type == REDIRECT_INPUT)
		redirect_input(data);
	else if (data->redirection_type == REDIRECT_OUTPUT)
		redirect_output(data);
	//else if (data->redirection_type == REDIRECT_HEREDOC)
		// to do
	else if (data->redirection_type == REDIRECT_APPEND)
		redirect_append(data);
}

void	execute(t_data *data, char ***envp)
{
	if (!data)
		return ;
	if (data->type == EXECUTION)
		exec(data, *envp);
	else if (data->type == PIPE)
		pipes(data, envp);
	else if (data->type == REDIRECTION)
		redirection(data);
	else if (data->type == BUILTIN)
		builtin(data, envp);
}

