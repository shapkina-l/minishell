/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 21:48:30 by lshapkin          #+#    #+#             */
/*   Updated: 2025/03/27 23:12:06 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

t_data *create_new_node()
{
    t_data *node;
    
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

    return (node);
}

int ft_strcmp(const char *s1, const char *s2)
{
    while (*s1 && *s2 && *s1 == *s2)
    {
        s1++;
        s2++;
    }
    return ((unsigned char)*s1 - (unsigned char)*s2);
}

int builtin_check(char *cmd)
{
    if (ft_strcmp(cmd, "echo") == 0)
        return(BUILTIN_ECHO);
    else if (ft_strcmp(cmd, "cd") == 0)
        return(BUILTIN_CD);
    else if (ft_strcmp(cmd, "export") == 0)
        return(BUILTIN_EXPORT);
    else if (ft_strcmp(cmd, "unset") == 0)
        return(BUILTIN_UNSET);
    else if (ft_strcmp(cmd, "env") == 0)
        return(BUILTIN_ENV);
    else if (ft_strcmp(cmd, "exit") == 0)
        return(BUILTIN_EXIT);
    else 
        return (-1);
}