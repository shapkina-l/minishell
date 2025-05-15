/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 00:10:15 by lshapkin          #+#    #+#             */
/*   Updated: 2025/05/04 00:32:35 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*add_token(t_token **head, t_token *current, t_token *new)
{
	if (!new)
		return (current);
	if (!new->value || !*new->value)
	{
		free(new->value);
		free(new);
		return (current);
	}
	if (!*head)
		*head = new;
	else
		current->next = new;
	new->next = NULL;
	return (new);
}

t_token	*tokenize(char *input, int *exit_status, char **my_envp)
{
	t_token	*head;
	t_token	*current;
	t_token	*new;

	head = NULL;
	current = NULL;
	if (!input || !*input)
		return (NULL);
	while (*input)
	{
		skip_whitespaces(&input);
		if (!*input)
			break ;
		if (ft_strchr("|<>", *input))
			new = tokenize_operator(&input);
		else
			new = tokenize_word(&input, exit_status, my_envp);
		current = add_token(&head, current, new);
	}
	return (head);
}
