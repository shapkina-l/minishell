/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:30:03 by lshapkin          #+#    #+#             */
/*   Updated: 2025/03/14 17:28:40 by lshapkin         ###   ########.fr       */
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

//types
//execution - 1
//pipe - 2
//redirection - 3
//builtin - 4

//builtin types
// 1 - echo with option -n
// 2 - cd with only a relative or absolute path
// 3 - pwd with no options
// 4 - export with no options
// 5 - unset with no options
// 6 - env with no options or arguments
// 7 - exit with no options

typedef struct s_data
{
	int				type;
	char			*full_cmd;
	char			**args;
	struct s_data	*left;
	struct s_data	*right;
	int				builtin_type;
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

#endif