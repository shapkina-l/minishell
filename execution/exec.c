/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:43:30 by lshapkin          #+#    #+#             */
/*   Updated: 2025/04/23 20:30:36 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exec(t_data *data)
{
	int	status;
	int	pid;

	// ✅ Add this check early: no args or empty command
	if (!data->args || !data->args[0] || data->args[0][0] == '\0')
		return (0); // Bash returns 0 when given nothing to execute

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
		if (access(data->full_cmd, X_OK) == -1) // check if the file exists and is executable before calling execve
		{
			if (access(data->full_cmd, F_OK) == 0)
				exit(0); // Bash does not complain for non-executable files without shebang
			else
			{
				perror(data->full_cmd);
				exit(126); // Permission denied
			}
		}
		execve(data->full_cmd, data->args, data->my_envp);
		if (errno == ENOEXEC)
			exit(0); // Bash: silent ignore if not a valid binary/script
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

int	has_output_redirection(t_data *data)
{
	if (!data)
		return (0);
	if (data->type == REDIRECTION &&
		(data->redirection_type == REDIRECT_OUTPUT || data->redirection_type == REDIRECT_APPEND))
		return (1);
	return (has_output_redirection(data->left) || has_output_redirection(data->right));
}

int	has_input_redirection(t_data *data)
{
	if (!data)
		return (0);
	if (data->type == REDIRECTION &&
		(data->redirection_type == REDIRECT_INPUT || data->redirection_type == REDIRECT_HEREDOC))
		return (1);
	return (has_input_redirection(data->left) || has_input_redirection(data->right));
}

void	exec_pipe(int fd[2], int var, t_data *exec, int *exit_status)
{
	int	ret;

	if (var == 1)
	{
		if (fd[1] != -1) // Only dup2 if no redirection
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

int	check_all_files(t_data *data)
{
	if (!data)
		return (0);
	if (data->left && check_all_files(data->left))
		return (1);
	if (data->right && check_all_files(data->right))
		return (1);
	if (data->type == REDIRECTION)
	{
		if (data->redirection_type == REDIRECT_HEREDOC)
			return (0);
		int fd = -1;
		if (data->redirection_type == REDIRECT_INPUT)
			fd = open(data->redirection_file, O_RDONLY);
		else if (data->redirection_type == REDIRECT_OUTPUT)
			fd = open(data->redirection_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (data->redirection_type == REDIRECT_APPEND)
			fd = open(data->redirection_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
		{
			return (1);
		}
		close(fd);
	}
	return (0);
}

void apply_redirection(t_data *redir, int *exit_status)
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

void apply_redirections(t_data *data, int *exit_status)
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

void	clean_exit_child(t_data *branch, int ret)
{
	free_exec(branch);
	rl_clear_history();
	exit(ret);
}

int	pipes(t_data *data, int *exit_status)
{
	int	fd[2];
	int	pid1 = -1;
	int	pid2 = -1;
	int	status1 = 0;
	int	status2 = 0;
	int check;

	check = special_case_export(data, fd, exit_status);
	if (check != -1)
		return (check);

	if (pipe(fd) == -1)
	{
		perror("pipe");
		return (1);
	}
	// LEFT child
	pid1 = fork();
	if (pid1 < 0)
		perror("fork");
	if (pid1 == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		signal(SIGPIPE, SIG_IGN); // ✅ avoid broken pipe signals
		if (data->left->type == REDIRECTION && check_all_files(data->left) == 0)
			apply_redirections(data->left, exit_status);
		// Redirect stdout to pipe only if no output redirection
		if (!has_output_redirection(data->left))
			dup2(fd[1], STDOUT_FILENO);
		close_fd(fd); // always close read end

		// ✅ redirections in child
		clean_exit_child(data->left, execute(data->left, exit_status));
	}
	// RIGHT child
	pid2 = fork();
	if (pid2 < 0)
		perror("fork");
	if (pid2 == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		signal(SIGPIPE, SIG_IGN); // ✅ avoid broken pipe signals

		if (data->right->type == REDIRECTION && check_all_files(data->right) == 0)
			apply_redirections(data->right, exit_status);
		// Redirect stdin to pipe only if no input redirection
		if (!has_input_redirection(data->right))
			dup2(fd[0], STDIN_FILENO);
		close_fd(fd); // always close write end
		clean_exit_child(data->right, execute(data->right, exit_status));
	}
	close_fd(fd);
	if (pid1 != -1)
		waitpid(pid1, &status1, 0);
	if (pid2 != -1)
		waitpid(pid2, &status2, 0);

	if (pid2 != -1 && WIFEXITED(status2))
		return (WEXITSTATUS(status2));
	if (pid1 != -1 && WIFEXITED(status1))
		return (WEXITSTATUS(status1));
	return (1);
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
	{
		t_data *command_node = get_command_node(data);
		if (!command_node) 
			return (1);
		if (check_all_files(data)) // ✅ don't run if redirection failed
			return (perror(data->redirection_file), 1);
		// ✅ Apply redirections here (non-pipe case)
		apply_redirections(data, exit_status);
		if (command_node->type == EXECUTION || command_node->type == BUILTIN)
			ret = execute(command_node, exit_status);
		else
			return (1); // suppress execution if invalid
	}
	else if (data->type == BUILTIN)
		ret = builtin(data, exit_status);
	reset_redirections(data->original_stdin, data->original_stdout);
	*exit_status = ret;
	return (ret);
}
