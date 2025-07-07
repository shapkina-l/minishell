/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:30:03 by lshapkin          #+#    #+#             */
/*   Updated: 2025/05/21 17:48:17 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <sys/wait.h>
# include <limits.h>
# include <sys/param.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/stat.h>
# include <errno.h>

# define BUF_SIZE 4096

//Single global variable with bit flags
// Bit 0: g_in_prompt, Bit 1: g_heredoc_sigint
extern int	g_shell_state;

typedef enum e_exec_type
{
	EXECUTION,
	PIPE,
	REDIRECTION,
	BUILTIN
}	t_exec_type;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_HEREDOC,
	TOKEN_APPEND,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_OPEN_PAREN,
	TOKEN_CLOSE_PAREN,
	TOKEN_EOF
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef enum e_builtin_type
{
	BUILTIN_ECHO,
	BUILTIN_CD,
	BUILTIN_PWD,
	BUILTIN_EXPORT,
	BUILTIN_UNSET,
	BUILTIN_ENV,
	BUILTIN_EXIT
}	t_builtin_type;

typedef enum e_redirection_type
{
	REDIRECT_INPUT,
	REDIRECT_OUTPUT,
	REDIRECT_HEREDOC,
	REDIRECT_APPEND
}	t_redirection_type;

typedef struct s_data
{
	int				type;
	char			*full_cmd;
	char			**args;
	struct s_data	*root;
	struct s_data	*left;
	struct s_data	*right;
	int				builtin_type;
	int				redirection_type;
	char			*redirection_file;
	int				end_flag;
	int				original_stdin;
	int				original_stdout;
	char			**my_envp;
	int				is_delimiter_quoted;
}	t_data;

typedef struct s_redirect_set
{
	char	*input_file;
	char	*output_file;
	char	*append_file;
}	t_redirect_set;

typedef struct s_pipes_utils
{
	int	fd[2];
	int	pid1;
	int	pid2;
	int	status1;
	int	status2;
	int	check;
}	t_pipes_utils;

typedef struct s_expand_utils
{
	const char	*line;
	char		*out;
	int			idx;
	int			pos;
	int			exit_status;
}	t_expand_utils;

typedef struct s_process_word_data
{
	int		*exit_status;
	char	*quote_char;
	char	**my_envp;
}	t_process_word_data;

int		execute(t_data *data, int *exit_status);
void	free_exec(t_data *data);
int		ft_echo(t_data *data);
int		ft_cd(t_data *data);
int		ft_pwd(void);
int		ft_export(t_data *data);
int		ft_unset(t_data *data);
int		ft_env(char *envp[]);
int		ft_exit(t_data *data, int *exit_status);
int		redirect_input(t_data *data);
int		redirect_output(t_data *data);
int		redirect_append(t_data *data);
t_token	*tokenize(char *input, int *exit_status, char **my_envp);
t_data	*parse_input(char *input, int *exit_status, char **my_envp);
char	*make_cmd(char **cmd_path, char *argv);
int		ft_strcmp(const char *s1, const char *s2);
t_data	*create_new_node(char **my_envp);
int		builtin_check(char *cmd);
void	*ft_realloc(void *ptr, size_t old_size, size_t new_size);
void	my_shell_handler(int signum);
void	reset_redirections(int original_stdin, int original_stdout);
void	skip_whitespaces(char **input);
int		handle_env_var(char **input, char *buffer,
			int buf_index, char **my_envp);
t_data	*parse_pipe(t_token *token, char **my_envp);
t_data	*parse_command(t_token *token, char **my_envp);
t_data	*parse_redirection(t_token *token, t_data *cmd_node, char **my_envp);
void	handle_all_args(t_token *token, t_data *node);
int		ft_isspace(char c);
void	print_error(const char *prefix, const char *msg);
void	exec_file_check(t_data *data);
void	close_fd(int fd[2]);
t_data	*find_last_redirection(t_data *data, int type);
t_data	*get_command_node(t_data *data);
int		builtin(t_data *data, int *exit_status);
int		handle_heredoc(t_data *data, int *exit_status, char **my_envp);
int		redirect_heredoc(t_data *data);
void	cleanup_heredoc_files(t_data *root);
void	free_envp(char **envp);
void	free_token_list(t_token *head);
void	exec_child_access_check(t_data *data);
int		pipes(t_data *data, int *exit_status);
char	*check_all_files(t_data *data);
void	apply_redirections(t_data *data);
int		has_output_redirection(t_data *data);
int		has_input_redirection(t_data *data);
int		exec(t_data *data);
t_token	*tokenize_operator(char **input);
t_token	*tokenize_word(char **input, int *exit_status, char **my_envp);
int		process_all_heredocs(t_data *node, int *exit_status, char **my_envp);
void	handle_heredoc_signal(int sig);
void	process_heredoc_lines(int fd, const char *delimiter,
			int *exit_status, char **my_envp);
int		prepare_heredoc(t_data *data, char **temp_file,
			char **delimiter);
int		handle_fork_error(char *temp_file, char *delimiter);
int		run_heredoc_child(const char *temp_file, const char *delimiter,
			int *exit_status, char **my_envp);
char	*create_heredoc_tempfile(void);
char	*expand_vars_in_line(const char *line, int exit_status, char **my_envp);
int		handle_fork_error(char *temp_file, char *delimiter);
char	*ft_readline(void);
long	ft_atol(const char *str);
int		is_numeric(char *str);
void	builtin_in_redirection(t_data *command_node,
			int *exit_status, int *ret, int pid);
char	**reset_redir_and_cleanup_heredoc(t_data *root, char **my_envp);
char	*get_env_value(const char *name, char **my_envp);
void	sort_envp(char **my_envp);
int		is_valid_identifier(char *name);

#endif
