/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 14:39:22 by lshapkin          #+#    #+#             */
/*   Updated: 2025/05/06 17:47:31 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

long	ft_atol(const char *str)
{
	int		sign;
	long	result;

	sign = 1;
	result = 0;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		sign = 44 - *str;
		str++;
	}
	while (*str && *str >= '0' && *str <= '9')
	{
		if (result > (LONG_MAX - (*str - '0')) / 10)
		{
			if (sign == 1)
				return (LONG_MAX);
			else
				return (LONG_MIN);
		}
		result = result * 10 + (*str - '0');
		str++;
	}
	return (sign * result);
}

int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (!str || !*str)
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (!isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_exit(t_data *data, int *exit_status)
{
	long	code;

	printf("exit\n");
	if (!data->args[1])
		exit(*exit_status);
	if (!is_numeric(data->args[1]))
	{
		write(STDERR_FILENO, "minishell: exit: ", 17);
		write(STDERR_FILENO, data->args[1], ft_strlen(data->args[1]));
		write(STDERR_FILENO, ": numeric argument required\n", 29);
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
