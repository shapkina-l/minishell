/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 19:01:51 by lshapkin          #+#    #+#             */
/*   Updated: 2025/04/07 18:05:04 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_echo(t_data *data)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	if (data->args[1]
		&& ft_strncmp(data->args[1], "-n", ft_strlen(data->args[1])) == 0)
	{
		newline = 0;
		i++;
	}
	while (data->args[i])
	{
		printf("%s", data->args[i]);
		if (data->args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}

int	ft_cd(t_data *data)
{
	char	*path;

	if (data->args[1])
	{
		if (data->args[2])
		{
			fprintf(stderr, "cd: too many arguments\n");
			return (1);
		}
		path = data->args[1];
	}
	else
		path = getenv("HOME");
	if (!path)
	{
		printf("cd: HOME not set\n");
		return (1);
	}
	if (chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

int	ft_pwd(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)))
		printf("%s\n", cwd);
	else
		perror("pwd");
	return (0);
}

int	ft_env(char *envp[])
{
	int	i;

	i = 0;
	while (envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
	return (0);
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
		if (*str == '-')
			sign = -1;
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

int	ft_exit(t_data *data, int *exit_status)
{
	long	code;

	printf("exit\n");
	if (!data->args[1]) // No arguments
		exit(*exit_status);
	if (!is_numeric(data->args[1])) // Non-numeric
	{
		fprintf(stderr, "minishell: exit: %s: numeric argument required\n", data->args[1]);
		exit(2);
	}
	if (data->args[2]) // Too many arguments
	{
		fprintf(stderr, "minishell: exit: too many arguments\n");
		*exit_status = 1;
		return (1); // Don't exit
	}
	code = ft_atol(data->args[1]); // Handles + / - and quotes
	exit((unsigned char)code);
}
