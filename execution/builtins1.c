/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 19:01:51 by lshapkin          #+#    #+#             */
/*   Updated: 2025/04/03 01:07:11 by lshapkin         ###   ########.fr       */
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
		path = data->args[1];
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

int	ft_exit(void)
{
	printf("exit\n");
	exit(0);
}
