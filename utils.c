/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 21:48:30 by lshapkin          #+#    #+#             */
/*   Updated: 2025/05/15 14:13:43 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*make_cmd_check(char *argv, struct stat file_stat)
{
	if (access(argv, F_OK) == 0)
	{
		if (stat(argv, &file_stat) == 0 && S_ISDIR(file_stat.st_mode))
			return (NULL);
		else if (access(argv, X_OK) == 0)
			return (ft_strdup(argv));
	}
	return (NULL);
}

char	*make_cmd(char **cmd_path, char *argv)
{
	char		*tmp;
	char		*cmd;
	struct stat	file_stat;

	if (!argv || !*argv || !cmd_path || !cmd_path[0])
		return (NULL);
	if (argv[0] == '/' || (argv[0] == '.'
			&& (argv[1] == '/' || (argv[1] == '.' && argv[2] == '/'))))
	{
		if (stat(argv, &file_stat) == 0)
			return (make_cmd_check(argv, file_stat));
		return (NULL);
	}
	while (*cmd_path)
	{
		tmp = ft_strjoin(*cmd_path, "/");
		cmd = ft_strjoin(tmp, argv);
		free (tmp);
		if (access(cmd, X_OK) == 0)
			return (cmd);
		free (cmd);
		cmd_path++;
	}
	return (NULL);
}

t_data	*create_new_node(char **my_envp)
{
	t_data	*node;

	node = malloc(sizeof(t_data));
	if (!node)
		return (NULL);
	node->type = -1;
	node->full_cmd = NULL;
	node->args = NULL;
	node->redirection_type = -1;
	node->redirection_file = NULL;
	node->is_delimiter_quoted = 0;
	node->left = NULL;
	node->right = NULL;
	node->original_stdin = dup(STDIN_FILENO);
	node->original_stdout = dup(STDOUT_FILENO);
	node->my_envp = my_envp;
	return (node);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

int	builtin_check(char *cmd)
{
	if (ft_strcmp(cmd, "echo") == 0)
		return (BUILTIN_ECHO);
	else if (ft_strcmp(cmd, "cd") == 0)
		return (BUILTIN_CD);
	else if (ft_strcmp(cmd, "pwd") == 0)
		return (BUILTIN_PWD);
	else if (ft_strcmp(cmd, "export") == 0)
		return (BUILTIN_EXPORT);
	else if (ft_strcmp(cmd, "unset") == 0)
		return (BUILTIN_UNSET);
	else if (ft_strcmp(cmd, "env") == 0)
		return (BUILTIN_ENV);
	else if (ft_strcmp(cmd, "exit") == 0)
		return (BUILTIN_EXIT);
	else
		return (-1);
}
