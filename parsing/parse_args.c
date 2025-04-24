/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 21:22:24 by lshapkin          #+#    #+#             */
/*   Updated: 2025/04/10 21:23:19 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_arguments(t_token *token, t_data *node)
{
	int		count_args;
	t_token	*tmp;

	count_args = 1;
	tmp = token->next;
	while (tmp && tmp->type == TOKEN_WORD)
	{
		tmp = tmp->next;
		count_args++;
	}
	node->args = malloc((count_args + 1) * sizeof(char *));
	if (!node->args)
	{
		free(node);
		return (0);
	}
	node->args[0] = ft_strdup(token->value);
	if (!node->args[0])
	{
		free(node->args);
		free(node);
		return (0);
	}
	return (count_args);
}

int	count_all_cmd_args(t_token *token)
{
	t_token	*tmp;
	int		count_args;

	count_args = 0;
	tmp = token;
	while (tmp && tmp->type != TOKEN_PIPE)
	{
		if (tmp->type == TOKEN_WORD && tmp->value && tmp->value[0] != '\0')
			count_args++;
		else if (tmp->type >= TOKEN_REDIRECT_IN && tmp->type <= TOKEN_APPEND)
		{
			if (tmp->next && tmp->next->type == TOKEN_WORD)
				tmp = tmp->next;
		}
		tmp = tmp->next;
	}
	return (count_args);
}

void	free_args_array(t_data *node, int i)
{
	while (i > 0)
		free(node->args[--i]);
	free(node->args);
	free(node);
}

void	fill_args_array(t_token *token, t_data *node, int count_args)
{
	t_token	*tmp;
	int		i;

	tmp = token;
	i = 0;
	while (tmp && tmp->type != TOKEN_PIPE && i < count_args)
	{
		if (tmp->type == TOKEN_WORD)
		{
			if (tmp->value[0] == '\0') // skip empty tokens like from $EMPTY
			{
				tmp = tmp->next;
				continue;
			}
			node->args[i] = ft_strdup(tmp->value);
			if (!node->args[i])
			{
				free_args_array(node, i);
				return ;
			}
			i++;
		}
		else if (tmp->type >= TOKEN_REDIRECT_IN && tmp->type <= TOKEN_APPEND)
		{
		// skip the next token if it's the redirection target (a WORD)
			if (tmp->next && tmp->next->type == TOKEN_WORD)
				tmp = tmp->next;
		}
		tmp = tmp->next;
	}
	node->args[i] = NULL;
}

void	handle_all_args(t_token *token, t_data *node)
{
	int		count_args;

	count_args = 0;
	count_args = count_all_cmd_args(token);
	node->args = malloc((count_args + 1) * sizeof(char *));
	if (!node->args)
	{
		free(node);
		return ;
	}
	fill_args_array(token, node, count_args);
	if (!node->args[0]) // All args got skipped (e.g., $EMPTY became "")
	{
		free(node->args);
		node->args = NULL;
		return ;
	}
}
