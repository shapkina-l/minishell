/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 19:27:54 by lshapkin          #+#    #+#             */
/*   Updated: 2025/04/23 20:46:12 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int g_heredoc_sigint = 0;

static void	handle_heredoc_signal(int sig)
{
	(void)sig;
	g_heredoc_sigint = 1;
	write(STDOUT_FILENO, "\n", 1);
	close(STDIN_FILENO);
}

static char	*expand_vars_in_line(char *line, int *exit_status)
{
	char	*expanded;
	char	*tmp;
	int		i;
	int		j;
	char	var_name[256];
	char	*var_value;
	
	expanded = malloc(4096); // Allocate sufficient space
	if (!expanded)
		return (NULL);
	
	i = 0;
	j = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1] == '?')
		{
			tmp = ft_itoa(*exit_status);
			ft_strlcpy(expanded + j, tmp, 4096 - j);
			j += ft_strlen(tmp);
			free(tmp);
			i += 2;
		}
		else if (line[i] == '$' && (ft_isalnum(line[i + 1]) || line[i + 1] == '_'))
		{
			i++; // Skip the $
			int var_len = 0;
			while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
				var_name[var_len++] = line[i++];
			var_name[var_len] = '\0';
			
			var_value = getenv(var_name);
			if (var_value)
			{
				ft_strlcpy(expanded + j, var_value, 4096 - j);
				j += ft_strlen(var_value);
			}
		}
		else
			expanded[j++] = line[i++];
	}
	expanded[j] = '\0';
	return (expanded);
}

char *create_heredoc_tempfile(void)
{
    char *filename;
    static int heredoc_count = 0;
    char count_str[20];
    
    // Convert heredoc count to string
    sprintf(count_str, "%d", heredoc_count++);
    
    // Create a unique temp filename
    filename = ft_strjoin("/tmp/minishell_heredoc_", count_str);
    return filename;
}

int is_quoted_delimiter(char *delimiter)
{
    // Check if the original token had quotes
    // For now, we'll assume that quotes were removed during tokenization
    // and that we have some flag or way to know if it was quoted
    
    // If you don't have this information yet, you'd need to modify your
    // tokenization to preserve this info
    
    // For this implementation, let's check if the delimiter starts with a quote
    // This is just a placeholder - you'll need to adapt this to your tokenizer
    if (delimiter && (delimiter[0] == '\'' || delimiter[0] == '"'))
        return 1;
    return 0;
}

int	handle_heredoc(t_data *data, int *exit_status)
{
	char	*line;
	char	*expanded_line;
	int		pid;
	int		status;
	char    *temp_file;
	int     fd;
	int     should_expand;
	char    *delimiter;
	
	// Create unique temp filename
	temp_file = create_heredoc_tempfile();
	if (!temp_file)
		return (1);
		
	// Determine if we should expand variables
	should_expand = !is_quoted_delimiter(data->redirection_file);
	
	// Store the delimiter (remove quotes if present)
	delimiter = ft_strdup(data->redirection_file);
	// Strip quotes if needed
	// This depends on how you handle quotes in your tokenizer
	
	pid = fork();
	if (pid == -1)
	{
		free(temp_file);
		free(delimiter);
		return (perror("fork"), 1);
	}
	
	if (pid == 0)
	{
		// Child process
		signal(SIGINT, handle_heredoc_signal);
		
		fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			perror("heredoc temp file");
			exit(1);
		}
		
		while (1)
		{
			line = readline("> ");
			if (!line || g_heredoc_sigint || ft_strcmp(line, delimiter) == 0)
			{
				free(line);
				break;
			}
			if (should_expand)
			{
				expanded_line = expand_vars_in_line(line, exit_status);
				free(line);
				
				if (!expanded_line)
				{
					close(fd);
					exit(1);
				}
					
				write(fd, expanded_line, ft_strlen(expanded_line));
				write(fd, "\n", 1);
				free(expanded_line);
			}
			else
			{
				// Don't expand variables
				write(fd, line, ft_strlen(line));
				write(fd, "\n", 1);
				free(line);
			}
		}
		
		close(fd);
		free(delimiter);
		free(temp_file);
		exit(g_heredoc_sigint ? 130 : 0);
	}
	
	// Parent process
	waitpid(pid, &status, 0);
	free(delimiter);
	
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
	{
		unlink(temp_file);
		free(temp_file);
		*exit_status = WEXITSTATUS(status);
		return (1);
	}
	
	// Store the temp file path for later use
	free(data->redirection_file);
	data->redirection_file = temp_file;
	
	return (0);
}

int	redirect_heredoc(t_data *data, int *exit_status)
{
	int fd;
	
	// First, process the heredoc if needed
	if (handle_heredoc(data, exit_status) != 0)
		return (1);
	
	// Open the temp file for reading
	fd = open(data->redirection_file, O_RDONLY);
	if (fd == -1)
	{
		perror("Error opening heredoc file");
		return (1);
	}
	
	// Redirect stdin to the temp file
	dup2(fd, STDIN_FILENO);
	close(fd);
	
	// Mark the temp file for cleanup
	// You might want to create a cleanup function 
	// that unlinks all temp files after execution
	
	return (0);
}

// Function to clean up heredoc temp files
void cleanup_heredoc_files(t_data *root)
{
	if (!root)
		return;
		
	if (root->type == REDIRECTION && 
		root->redirection_type == REDIRECT_HEREDOC && 
		root->redirection_file)
	{
		// Only unlink if it's a temp file
		if (ft_strncmp(root->redirection_file, "/tmp/minishell_heredoc_", 22) == 0)
			unlink(root->redirection_file);
	}
	
	cleanup_heredoc_files(root->left);
	cleanup_heredoc_files(root->right);
}