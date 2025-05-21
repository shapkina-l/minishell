/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 21:30:23 by lshapkin          #+#    #+#             */
/*   Updated: 2025/05/21 14:24:37 by lshapkin         ###   ########.fr       */
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
	g_shell_state = g_shell_state & ~1;
	if (input)
		add_history(input);
	return (input);
}

void	reset_redir_and_cleanup_heredoc(t_data *root)
{
	reset_redirections(root->original_stdin, root->original_stdout);
	cleanup_heredoc_files(root);
}

void	sort_envp(char **my_envp)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (my_envp && my_envp[i])
	{
		j = i + 1;
		while (my_envp[j])
		{
			if (ft_strncmp(my_envp[i], my_envp[j],
					ft_strlen(my_envp[i]) + 1) > 0)
			{
				tmp = my_envp[i];
				my_envp[i] = my_envp[j];
				my_envp[j] = tmp;
			}
			j++;
		}
		i++;
	}
}
