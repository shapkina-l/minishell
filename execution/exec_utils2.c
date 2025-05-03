/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 14:39:22 by lshapkin          #+#    #+#             */
/*   Updated: 2025/05/03 20:45:52 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	special_case_exp_child(t_data *data, int fd[2], int *exit_status, int ret)
{
	dup2(fd[0], STDIN_FILENO);
	close_fd(fd);
	ret = execute(data->right, exit_status);
	exit(ret);
}

int	special_case_export(t_data *data, int fd[2], int *exit_status)
{
	int	ret;
	int	pid2;
	int	status2;

	if (data->left->type == BUILTIN
		&& (data->left->builtin_type == BUILTIN_EXPORT
			|| data->left->builtin_type == BUILTIN_UNSET))
	{
		ret = builtin(data->left, exit_status);
		if (pipe(fd) == -1)
			perror("pipe");
		pid2 = fork();
		if (pid2 < 0)
			perror("fork");
		if (pid2 == 0)
			special_case_exp_child(data, fd, exit_status, ret);
		close_fd(fd);
		waitpid(pid2, &status2, 0);
		if (WIFEXITED(status2))
			return (WEXITSTATUS(status2));
		return (1);
	}
	return (-1);
}

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
		fprintf(stderr, "minishell: exit: %s: numeric argument required\n",
			data->args[1]);
		exit(2);
	}
	if (data->args[2])
	{
		fprintf(stderr, "minishell: exit: too many arguments\n");
		*exit_status = 1;
		return (1);
	}
	code = ft_atol(data->args[1]);
	exit((unsigned char)code);
}
