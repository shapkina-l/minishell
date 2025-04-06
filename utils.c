/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 21:48:30 by lshapkin          #+#    #+#             */
/*   Updated: 2025/04/06 21:29:55 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*make_cmd(char **cmd_path, char *argv) //needed to be adapted for the case when PATH=""
												// that should then bring a command call to cause an error
{
	char	*tmp;
	char	*cmd;

	if (!argv || !*argv)
		return (NULL);

	// Case 2: check if PATH is empty
	if (!cmd_path || !cmd_path[0])
		return (NULL);
		
	// Case 1: absolute or relative path
	if (argv[0] == '/' || (argv[0] == '.' && argv[1] == '/'))
	{
		if (access(argv, X_OK) == 0)
			return (ft_strdup(argv));
		else
			return (NULL);
	}

	// Case 3: search in PATH directories
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
	node->args = NULL;
	node->full_cmd = NULL;
	node->redirection_type = 0;
	node->redirection_file = NULL;
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

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;
	size_t	copy_size;

	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	new_ptr = malloc(new_size);
	if (!new_ptr)
		return (NULL);
	if (ptr)
	{
		if (old_size < new_size)
			copy_size = old_size;
		else
			copy_size = new_size;
		ft_memcpy(new_ptr, ptr, copy_size);
		free(ptr);
	}
	return (new_ptr);
}
