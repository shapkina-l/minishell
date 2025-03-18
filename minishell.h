/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:30:03 by lshapkin          #+#    #+#             */
/*   Updated: 2025/03/18 15:48:53 by lshapkin         ###   ########.fr       */
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


typedef enum e_token_type 
{
	TOKEN_WORD,         // Regular command/argument
    TOKEN_PIPE,         // '|'
    TOKEN_REDIRECT_IN,  // '<'
    TOKEN_REDIRECT_OUT, // '>'
    TOKEN_HEREDOC,      // '<<'
    TOKEN_APPEND,       // '>>'
    TOKEN_AND,         // '&&'
    TOKEN_OR,          // '||'
    TOKEN_OPEN_PAREN,  // '('
    TOKEN_CLOSE_PAREN, // ')'
    TOKEN_EOF          // End of input
} t_token_type;

typedef struct s_token
{
	t_token_type type;
	char *value;
	struct s_token *next;
} t_token;

//types for execution
//execution - 1
//pipe - 2
//redirection - 3
//builtin - 4

//builtin types
// 1 - echo with option -n
// 2 - cd with only a relative or absolute path
// 3 - pwd with no options
// 4 - export with no options !during parsing need to make sure that syntaxsis is corrext (=)!
// 5 - unset with no options
// 6 - env with no options or arguments
// 7 - exit with no options

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
	struct s_data	*left;
	struct s_data	*right;
	int				builtin_type;
	int				redirection_type;
	char			*redirection_file;
	int				end_flag;
}	t_data;

void	execute(t_data *data, char *envp[]);
void	free_exec(t_data *data); //temp
int		ft_echo(t_data *data);
int		ft_cd(t_data *data);
int		ft_pwd();
int		ft_export(t_data *data, char *envp[]);
int		ft_unset(t_data *data, char *envp[]);
int		ft_env(char *envp[]);
int		ft_exit();
void	redirect_input(t_data *data);
void	redirect_output(t_data *data);
void	redirect_append(t_data *data);

#endif