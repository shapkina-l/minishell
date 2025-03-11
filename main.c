/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:29:46 by lshapkin          #+#    #+#             */
/*   Updated: 2025/03/11 19:33:26 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*envp_path(char *envp[])
{
	while (*envp)
	{
		if (ft_strncmp("PATH=", *envp, 5) == 0)
			return (*envp + 5);
		envp++;
	}
	return (NULL);
}

char	*make_cmd(char **cmd_path, char *argv)
{
	char	*tmp;
	char	*cmd;

	if (access(argv, 0) == 0)
		return (argv);
	while (*cmd_path)
	{
		tmp = ft_strjoin(*cmd_path, "/");
		cmd = ft_strjoin(tmp, argv);
		free (tmp);
		if (access(cmd, 0) == 0)
			return (cmd);
		free (cmd);
		cmd_path++;
	}
	return (NULL);
}

// //Temporary data initialization for exec test
// void	tmp_fill_data(t_data *data, char *envp[])
// {
// 	char	*path = envp_path(envp);
// 	char	**cmd_path = ft_split(path, ':');
// 	char	*cmd = make_cmd(cmd_path, "ls");

// 	data->type = 1;
// 	data->full_cmd = cmd;
// 	data->args = ft_split("ls -la", ' ');
// 	data->left = NULL;
// 	data->right = NULL;
// 	int i = 0;
// 	while (cmd_path[i])
// 	{
// 		free (cmd_path[i]);
// 		i++;
// 	}
// 	free(cmd_path);
// }

//Temporary data initialization for pipes test ps aux | grep bash
void	tmp_fill_data(t_data *data, char *envp[])
{
	char	*path = envp_path(envp);
	char	**cmd_path = ft_split(path, ':');
	char	*cmd1 = make_cmd(cmd_path, "ps");
	char	*cmd2 = make_cmd(cmd_path, "grep");

	data->type = 2;

	data->left->type = 1;
	data->left->full_cmd = cmd1;
	data->left->args = ft_split("ps aux", ' ');
	data->left->left = NULL;
	data->left->right = NULL;

	data->right->type = 1;
	data->right->full_cmd = cmd2;
	data->right->args = ft_split("grep bash", ' ');
	data->right->left = NULL;
	data->right->right = NULL;
	int i = 0;
	while (cmd_path[i])
	{
		free (cmd_path[i]);
		i++;
	}
	free(cmd_path);
}

//Temporary main for execution
int	main(int argc, char *argv[], char *envp[])
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (0);
	data->left = malloc(sizeof(t_data)); //if!
	data->right = malloc(sizeof(t_data)); //if!
	tmp_fill_data(data, envp);
	execute(data, envp);

	//int i = 0;
	//while (data->args[i])
	//{
	//	free (data->args[i]);
	//	i++;
	//}
	free(data->args);
	free(data->full_cmd);
	free(data);
}
