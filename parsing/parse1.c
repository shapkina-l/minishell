/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 17:17:56 by apaz-mar          #+#    #+#             */
/*   Updated: 2025/05/15 14:11:12 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_env_value(const char *name, char **my_envp)
{
	int		i;
	int		len;

	i = 0;
	len = ft_strlen(name);
	while (my_envp[i])
	{
		if (ft_strncmp(my_envp[i], name, len) == 0 && my_envp[i][len] == '=')
			return (my_envp[i] + len + 1);
		i++;
	}
	return (NULL);
}

void	node_full_cmd(t_token *token, t_data *node)
{
	char	*path;
	char	**cmd_path;
	char	*full_cmd;
	int		i;

	path = NULL;
	cmd_path = NULL;
	full_cmd = NULL;
	i = 0;
	path = get_env_value("PATH", node->my_envp);
	if (!path || !*path)
		printf("Path is empty string\n");
	cmd_path = ft_split(path, ':');
	full_cmd = make_cmd(cmd_path, token->value);
	if (cmd_path)
	{
		while (cmd_path[i])
			free(cmd_path[i++]);
		free(cmd_path);
	}
	if (!full_cmd)
		node->full_cmd = NULL;
	node->full_cmd = full_cmd;
}

t_data	*parse_command(t_token *token, char **my_envp)
{
	t_data	*node;
	int		builtin;

	if (!token)
		return (NULL);
	node = create_new_node(my_envp);
	if (!node)
		return (NULL);
	builtin = builtin_check(token->value);
	if (builtin != -1)
	{
		node->type = BUILTIN;
		node->builtin_type = builtin;
	}
	else
		node->type = EXECUTION;
	if (node->type == EXECUTION)
		node_full_cmd(token, node);
	handle_all_args(token, node);
	return (node);
}

void	set_redirecton_type(t_token *tmp, t_data *redir_node)
{
	redir_node->type = REDIRECTION;
	if (tmp->type == TOKEN_REDIRECT_IN)
		redir_node->redirection_type = REDIRECT_INPUT;
	else if (tmp->type == TOKEN_REDIRECT_OUT)
		redir_node->redirection_type = REDIRECT_OUTPUT;
	else if (tmp->type == TOKEN_APPEND)
		redir_node->redirection_type = REDIRECT_APPEND;
	else if (tmp->type == TOKEN_HEREDOC)
		redir_node->redirection_type = REDIRECT_HEREDOC;
	redir_node->redirection_file = ft_strdup(tmp->next->value);
	redir_node->is_delimiter_quoted = 0;
}

t_data	*parse_redirection(t_token *token, t_data *cmd_node, char **my_envp)
{
	t_data	*redir_node;
	t_token	*tmp;
	t_data	*result;

	result = cmd_node;
	tmp = token;
	while (tmp && tmp->type != TOKEN_PIPE)
	{
		if (tmp->type >= TOKEN_REDIRECT_IN && tmp->type <= TOKEN_APPEND)
		{
			if (!tmp->next || tmp->next->type != TOKEN_WORD)
				return (NULL);
			redir_node = create_new_node(my_envp);
			if (!redir_node)
				return (NULL);
			set_redirecton_type(tmp, redir_node);
			if (!redir_node->redirection_file)
				return (NULL);
			redir_node->left = result;
			result = redir_node;
			tmp = tmp->next;
		}
		tmp = tmp->next;
	}
	return (result);
}
