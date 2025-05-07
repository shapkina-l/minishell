/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 21:30:23 by lshapkin          #+#    #+#             */
/*   Updated: 2025/05/07 23:38:47 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_fork_error(char *temp_file, char *delimiter)
{
	free(temp_file);
	free(delimiter);
	perror("fork");
	return (1);
}

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
