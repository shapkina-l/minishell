/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:30:03 by lshapkin          #+#    #+#             */
/*   Updated: 2025/03/29 16:33:42 by apaz-mar         ###   ########.fr       */
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

typedef enum e_exec_type
{
    EXECUTION,
    PIPE,
    REDIRECTION,
    BUILTIN
} t_exec_type;

typedef enum e_token_type 
{
	TOKEN_WORD,         // Regular command/argument
    TOKEN_PIPE,         // '|'
    TOKEN_REDIRECT_IN,  // '<'
    TOKEN_REDIRECT_OUT, // '>'
    TOKEN_HEREDOC,      // '<<'
    TOKEN_APPEND,       // '>>'
    TOKEN_AND,         // '&&' - bonus
    TOKEN_OR,          // '||' - bonus
    TOKEN_OPEN_PAREN,  // '(' - bonus
    TOKEN_CLOSE_PAREN, // ')' - bonus
    TOKEN_EOF          // End of input
} t_token_type;

typedef struct s_token
{
	t_token_type type;
	char *value;
	struct s_token *next;
} t_token;

typedef enum e_builtin_type
{
    BUILTIN_ECHO,
    BUILTIN_CD,
    BUILTIN_PWD,
    BUILTIN_EXPORT,
	BUILTIN_UNSET,
	BUILTIN_ENV,
	BUILTIN_EXIT
} t_builtin_type;

typedef enum e_redirection_type
{
    REDIRECT_INPUT,
    REDIRECT_OUTPUT,
    REDIRECT_HEREDOC,
    REDIRECT_APPEND
} t_redirection_type;

//redirection types
// 1 - < - should redirect input.
// 2 - > - should redirect output.
// 3 - << - should be given a delimiter, then read the input until a line containing the
//delimiter is seen. However, it doesn’t have to update the history!
// 4 - >> - should redirect output in append mode.


#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_PINK    "\x1b[35m"
#define ANSI_COLOR_RESET   "\x1b[0m"


typedef struct s_data //break into type-left-right and different struct for each?
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
}	t_data;

void	execute(t_data *data, char ***envp);
void	free_exec(t_data *data); //temp
int		ft_echo(t_data *data);
int		ft_cd(t_data *data);
int		ft_pwd();
int		ft_export(t_data *data, char ***envp);
int		ft_unset(t_data *data, char ***envp);
int		ft_env(char *envp[]);
int		ft_exit();
void	redirect_input(t_data *data);
void	redirect_output(t_data *data);
void	redirect_append(t_data *data);
t_token *tokenize(char *input);
t_data	*parse_input(char *input);
char	*make_cmd(char **cmd_path, char *argv);
int     ft_strcmp(const char *s1, const char *s2);
t_data *create_new_node();
int     builtin_check(char *cmd);
void    *ft_realloc(void *ptr, size_t old_size, size_t new_size);

#endif
