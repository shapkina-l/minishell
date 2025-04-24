/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 15:16:26 by lshapkin          #+#    #+#             */
/*   Updated: 2025/04/23 20:15:52 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_in_prompt = 0; // global variable must be defined in a function to allocate space for it

char	*ft_readline(void)
{
	char	*input;
	char	*prompt;

	g_in_prompt = 1;
	prompt = "minishell>";
	input = readline(prompt);
	g_in_prompt = 0;
	if (input)
		add_history(input);
	return (input);
}

//copy of the environment to avoid undefined behaviour of non allocated envp
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
	if (data->args) //free the args array
	{
		i = 0;
		while (data->args[i])
			free(data->args[i++]);
		free(data->args);
	}
	if (data->full_cmd) //free command string
		free(data->full_cmd);
	if (data->redirection_file) //free redirection file name
		free(data->redirection_file);
	if (data->left) // recursively free left and right branches
		free_exec(data->left);
	if (data->right)
		free_exec(data->right);
	free(data); //free the node itself
}

int	main(int argc, char **argv, char *envp[])
{
	char	*input;
	t_data	*root;
	char	**my_envp;
	int		last_exit_status;

	(void)argc;
	(void)argv;
	last_exit_status = 0;
	signal(SIGINT, my_shell_handler); // dont kill minishell (parent) with ctrl + C
	signal(SIGQUIT, SIG_IGN); // ignore Ctrl + \ in minishell (parent)
	my_envp = dup_envp(envp);
	if (!my_envp)
	{
		printf("Failed to initialize environment.\n");
		return (1);
	}
	while (1) //data->end_flag == 0
	{
		input = ft_readline();
		if (!input)
			break ;
		root = parse_input(input, &last_exit_status, my_envp);
		if (!root)
		{
			free(input);
			continue ; // it skips execute and free if root fails
		}
		last_exit_status = execute(root, &last_exit_status);
		cleanup_heredoc_files(root);
		my_envp = root->my_envp;
		free(input);
		free_exec(root);
	}
	free_envp(my_envp);
	return (last_exit_status);
}
