/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 15:24:27 by lshapkin          #+#    #+#             */
/*   Updated: 2025/05/21 14:44:48 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_unset_loop(t_data *data, int len)
{
	int	i;
	int	j;

	i = 0;
	while (data->args[i])
	{
		len = ft_strlen(data->args[i]);
		j = 0;
		while ((data->my_envp)[j])
		{
			if (ft_strncmp((data->my_envp)[j], data->args[i], len) == 0
				&& (data->my_envp)[j][strlen(data->args[i])] == '=')
			{
				free((data->my_envp)[j]);
				while ((data->my_envp)[j])
				{
					(data->my_envp)[j] = (data->my_envp)[j + 1];
					j++;
				}
				break ;
			}
			j++;
		}
		i++;
	}
}

int	ft_unset(t_data *data)
{
	int	len;

	len = 0;
	if (!data->args[1])
		return (0);
	ft_unset_loop(data, len);
	return (0);
}

int	ft_exit(t_data *data, int *exit_status)
{
	long	code;

	printf("exit\n");
	if (!data->args[1])
		exit(*exit_status);
	if (!is_numeric(data->args[1]))
	{
		print_error("minishell: exit", "numeric argument required");
		exit(2);
	}
	if (data->args[2])
	{
		write(STDERR_FILENO, "minishell: exit: too many arguments\n", 36);
		*exit_status = 1;
		return (1);
	}
	code = ft_atol(data->args[1]);
	exit((unsigned char)code);
}

int	is_valid_identifier(char *name)
{
	int	i;

	if (!name || !*name)
		return (0);
	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (0);
	i = 1;
	while (name[i] && name[i] != '=')
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
