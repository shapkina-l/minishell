/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 21:30:23 by lshapkin          #+#    #+#             */
/*   Updated: 2025/05/15 14:26:40 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_shell_state = 0;

int	handle_fork_error(char *temp_file, char *delimiter)
{
	free(temp_file);
	free(delimiter);
	perror("fork");
	return (1);
}

//~ NOT operator, from 0001 to 1110, then an AND operator results in 0
char	*ft_readline(void)
{
	char	*input;
	char	*prompt;

	g_shell_state = g_shell_state | 1;
	prompt = "minishell>";
	input = readline(prompt);
	g_shell_state = g_shell_state & -1;
	if (input)
		add_history(input);
	return (input);
}

void	reset_redir_and_cleanup_heredoc(t_data *root)
{
	reset_redirections(root->original_stdin, root->original_stdout);
	cleanup_heredoc_files(root);
}
