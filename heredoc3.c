/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 19:18:48 by lshapkin          #+#    #+#             */
/*   Updated: 2025/05/06 19:41:45 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define BUF_SIZE 4096

char	*create_heredoc_tempfile(void)
{
	char		*filename;
	static int	heredoc_count;
	char		count_str[20];

	heredoc_count = 0;
	sprintf(count_str, "%d", heredoc_count++);
	filename = ft_strjoin("/tmp/minishell_heredoc_", count_str);
	return (filename);
}

int	is_quoted_delimiter(char *delimiter)
{
	if (delimiter && (delimiter[0] == '\'' || delimiter[0] == '"'))
		return (1);
	return (0);
}

int	handle_fork_error(char *temp_file, char *delimiter)
{
	free(temp_file);
	free(delimiter);
	perror("fork");
	return (1);
}

int try_expand_exit(const char *line, int *idx, char *out, int *pos, int exit_status)
{
	char	*tmp;

	if (line[*idx] == '$' && line[*idx + 1] == '?')
	{
		tmp = ft_itoa(exit_status);
		ft_strlcpy(out + *pos, tmp, BUF_SIZE - *pos);
		*pos += ft_strlen(tmp);
		free(tmp);
		*idx += 2;
		return (1);
	}
	return (0);
}

void	expand_env_var(const char *line, int *idx, char *out, int *pos)
{
	char	var_name[256];
	int		len;
	char	*val;

	len = 0;
	(*idx)++;
	while (line[*idx] && (ft_isalnum(line[*idx]) || line[*idx] == '_'))
		var_name[len++] = line[(*idx)++];
	var_name[len] = '\0';
	val = getenv(var_name);
	if (val)
	{
		ft_strlcpy(out + *pos, val, BUF_SIZE - *pos);
		*pos += ft_strlen(val);
	}
}

void copy_char(const char *line, int *idx, char *out, int *pos)
{
	out[(*pos)++] = line[(*idx)++];
}

char *expand_vars_in_line(const char *line, int *exit_status)
{
	char	*out;
	int		i;
	int		j;

	out = malloc(BUF_SIZE);
	if (!out)
		return (NULL);
	i = 0;
	j = 0;
	while (line[i])
	{
		if (try_expand_exit(line, &i, out, &j, *exit_status))
			continue ;
		if (line[i] == '$' && (ft_isalnum(line[i + 1]) || line[i + 1] == '_'))
			expand_env_var(line, &i, out, &j);
		else
			copy_char(line, &i, out, &j);
	}
	out[j] = '\0';
	return (out);
}
