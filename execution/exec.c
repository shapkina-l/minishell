/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:43:30 by lshapkin          #+#    #+#             */
/*   Updated: 2025/04/07 21:28:46 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exec(t_data *data)
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
		execve(data->full_cmd, data->args, data->my_envp);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	waitpid(pid, &status, 0);
	return (WIFEXITED(status) ? WEXITSTATUS(status) : 1);
//	free_exec(data); //? --> freed in main
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

void	close_fd(int fd[2])
{
	if (fd[0] != -1)
		close(fd[0]);
	if (fd[1] != -1)
		close(fd[1]);
}

void	exec_pipe(int fd[2], int var, t_data *exec, int *exit_status)
{
	int	ret;

	if (var == 1)
	{
		dup2(fd[1], STDOUT_FILENO);
		close_fd(fd);
		ret = execute(exec, exit_status);
		exit(ret);
	}
	else if (var == 2)
	{
		dup2(fd[0], STDIN_FILENO);
		close_fd(fd);
		ret = execute(exec, exit_status);
		exit(ret);
	}
}

int	pipes(t_data *data, int *exit_status)
{
	int	fd[2];
	int	pid1;
	int	pid2;
	int	status1;
	int	status2;
	int ret;
	
	//move to separate function
    // Special case for export/unset on left side of pipe
    if (data->left->type == BUILTIN && 
        (data->left->builtin_type == BUILTIN_EXPORT ||
         data->left->builtin_type == BUILTIN_UNSET))
    {
        // Execute the builtin in the current process to update the environment
        ret = builtin(data->left, exit_status);
        
        // Now proceed with the pipe for the right side
        if (pipe(fd) == -1)
            perror("pipe");
            
        pid2 = fork();
        if (pid2 < 0)
            perror("fork");
        if (pid2 == 0)
        {
            // We only need to get input from pipe if there was output from left
            close(fd[1]);
            dup2(fd[0], STDIN_FILENO);
            close(fd[0]);
            ret = execute(data->right, exit_status);
            exit(ret);
        }
        close(fd[0]);
        close(fd[1]);
        waitpid(pid2, &status2, 0);
        if (WIFEXITED(status2))
            return (WEXITSTATUS(status2));
        return (1);
    }
	if (pipe(fd) == -1)
		perror("pipe");
	pid1 = fork();
	if (pid1 < 0)
		perror("fork");
	if (pid1 == 0)
		exec_pipe(fd, 1, data->left, exit_status);
	pid2 = fork();
	if (pid2 == 0)
		exec_pipe(fd, 2, data->right, exit_status);
	close_fd(fd);
	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	if (WIFEXITED(status2))
		return (WEXITSTATUS(status2));
	return (1);
}

int	redirection(t_data *data, int *exit_status)
{
	if (data->redirection_type == REDIRECT_INPUT)
		redirect_input(data);
	else if (data->redirection_type == REDIRECT_OUTPUT)
		redirect_output(data);
	//else if (data->redirection_type == REDIRECT_HEREDOC)
		// to do
	else if (data->redirection_type == REDIRECT_APPEND)
		redirect_append(data);
	return (execute(data->left, exit_status));
}

int	execute(t_data *data, int *exit_status)
{
	int	ret;

	ret = 1;
	if (!data)
		return (1);
	if (data->type == EXECUTION)
		ret = exec(data);
	else if (data->type == PIPE)
		ret = pipes(data, exit_status);
	else if (data->type == REDIRECTION)
		ret = redirection(data, exit_status);
	else if (data->type == BUILTIN)
		ret = builtin(data, exit_status);
	reset_redirections(data->original_stdin, data->original_stdout);
	*exit_status = ret;
	return (ret);
}

