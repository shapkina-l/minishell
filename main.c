/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 15:16:26 by lshapkin          #+#    #+#             */
/*   Updated: 2025/05/15 14:25:03 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**dup_envp(char **envp)
{
	int		i;
	int		len;
	char	**new_env;

	i = 0;
	len = 0;
	while (envp[len])
		len++;
	new_env = malloc(sizeof(char *) * (len + 1));
	if (!new_env)
		return (NULL);
	while (envp[i])
	{
		new_env[i] = ft_strdup(envp[i]);
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

void	free_envp(char **envp)
{
	int		i;

	i = 0;
	while (envp[i])
		free(envp[i++]);
	free(envp);
}

void	free_exec(t_data *data)
{
	int		i;

	if (!data)
		return ;
	if (data->args)
	{
		i = 0;
		while (data->args[i])
			free(data->args[i++]);
		free(data->args);
	}
	if (data->full_cmd)
		free(data->full_cmd);
	if (data->redirection_file)
		free(data->redirection_file);
	if (data->left)
		free_exec(data->left);
	if (data->right)
		free_exec(data->right);
	free(data);
}

void	main_loop(t_data *root, int *last_exit_status, char **my_envp)
{
	char	*input;

	while (1)
	{
		input = ft_readline();
		if (!input)
			break ;
		root = parse_input(input, last_exit_status, my_envp);
		if (!root)
		{
			free(input);
			continue ;
		}
		if (process_all_heredocs(root, last_exit_status, my_envp))
		{
			cleanup_heredoc_files(root);
			free_exec(root);
			continue ;
		}
		*last_exit_status = execute(root, last_exit_status);
		reset_redir_and_cleanup_heredoc(root);
		my_envp = root->my_envp;
		free(input);
		free_exec(root);
	}
}

int	main(int argc, char **argv, char *envp[])
{
	t_data	*root;
	char	**my_envp;
	int		last_exit_status;

	(void)argc;
	(void)argv;
	root = NULL;
	last_exit_status = 0;
	signal(SIGINT, my_shell_handler);
	signal(SIGQUIT, SIG_IGN);
	my_envp = dup_envp(envp);
	if (!my_envp)
	{
		printf("Failed to initialize environment.\n");
		return (1);
	}
	main_loop(root, &last_exit_status, my_envp);
	free_envp(my_envp);
	rl_clear_history();
	return (last_exit_status);
}
