/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:43:30 by lshapkin          #+#    #+#             */
/*   Updated: 2025/03/16 15:07:13 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exec(t_data *data, char *envp[])
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
	free_exec(data); //?
}

void	builtin(t_data *data, char *envp[])
{
	// 1 - echo with option -n
	if (data->builtin_type == 1)
		ft_echo(data);
	// 2 - cd with only a relative or absolute path
	else if (data->builtin_type == 2)
		ft_cd(data);
	// 3 - pwd with no options
	else if (data->builtin_type == 3)
		ft_pwd();
	// 4 - export with no options
	else if (data->builtin_type == 4)
		ft_export(data, envp);
	// 5 - unset with no options
	else if (data->builtin_type == 5)
		ft_unset(data, envp);
	// 6 - env with no options or arguments
	else if (data->builtin_type == 6)
		ft_env(envp);
	// 7 - exit with no options
	else if (data->builtin_type == 7)
		ft_exit();
}

void	pipes(t_data *data, char *envp[])
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

void	redirection(t_data *data, char *envp[])
{
	// 1 - < - should redirect input.
	if (data->redirection_type == 1)
		redirect_input(data);
	// 2 - > - should redirect output.
	else if (data->redirection_type == 2)
		redirect_output(data);
	// 3 - << - should be given a delimiter, then read the input until a line containing the
	//delimiter is seen. However, it doesn’t have to update the history!
	//else if (data->redirection_type == 3)
		// to do
	// 4 - >> - should redirect output in append mode.
	else if (data->redirection_type == 4)
		redirect_append(data);
}

void	execute(t_data *data, char *envp[])
{
	if (!data)
		return ;
	//Execution of a single command
	if (data->type == 1)
		exec(data, envp);
	//Pipes 
	else if (data->type == 2)
		pipes(data, envp);
	//Redirection
	else if (data->type == 3)
		redirection(data, envp);
	//Builtins
	else if (data->type == 4)
		builtin(data, envp);
}

