/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 15:24:27 by lshapkin          #+#    #+#             */
/*   Updated: 2025/05/03 15:25:15 by lshapkin         ###   ########.fr       */
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
