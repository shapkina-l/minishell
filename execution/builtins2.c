/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 19:01:51 by lshapkin          #+#    #+#             */
/*   Updated: 2025/05/03 15:24:54 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

int	ft_export_var_exists(char *new_var, char *envp[])
{
	int		j;
	char	*eq;
	size_t	len;
	char	*new_value;

	eq = ft_strchr(new_var, '=');
	if (!eq)
		return (-1);
	len = eq - new_var;
	j = 0;
	while (envp[j])
	{
		if (ft_strncmp(envp[j], new_var, len) == 0 && envp[j][len] == '=')
		{
			new_value = ft_strdup(new_var);
			if (!new_value)
				return (-1);
			free(envp[j]);
			envp[j] = new_value;
			return (1);
		}
		j++;
	}
	return (0);
}

int	ft_export_var_create(char *new_var, t_data *data)
{
	int		len;
	char	**new_envp;
	char	*dup_var;
	int		i;

	if (!data->my_envp)
		return (-1);
	len = 0;
	while ((data->my_envp)[len])
		len++;
	new_envp = malloc(sizeof(char *) * (len + 2));
	if (!new_envp)
		return (-1);
	i = 0;
	while (i < len)
	{
		new_envp[i] = data->my_envp[i];
		i++;
	}
	dup_var = ft_strdup(new_var);
	if (!dup_var)
		return (free(new_envp), -1);
	new_envp[len] = dup_var;
	new_envp[len + 1] = NULL;
	return (free(data->my_envp), data->my_envp = new_envp, 0);
}

int	ft_export_loop(t_data *data, int i, int var_exists)
{
	char	*var_name;
	char	*equals_pos;

	while (data->args[i])
	{
		var_name = ft_strdup(data->args[i]);
		if (!var_name)
			return (1);
		equals_pos = ft_strchr(var_name, '=');
		if (equals_pos)
			*equals_pos = '\0';
		if (!is_valid_identifier(var_name))
		{
			write(STDERR_FILENO, "export: `", 9);
			write(STDERR_FILENO, data->args[i], ft_strlen(data->args[i]));
			write(STDERR_FILENO, "': not a valid identifier\n", 27);
			free(var_name);
			return (1);
		}
		free(var_name);
		if (ft_strchr(data->args[i], '='))
		{
			var_exists = ft_export_var_exists(data->args[i], data->my_envp);
			if (var_exists < 0 || (var_exists == 0
					&& ft_export_var_create(data->args[i], data) < 0))
				return (1);
		}
		i++;
	}
	return (0);
}

int	ft_export(t_data *data)
{
	int		i;
	int		var_exists;

	var_exists = 0;
	if (!data->my_envp)
		return (1);
	if (!data->args[1])
	{
		i = 0;
		while ((data->my_envp)[i])
			printf("declare -x %s\n", (data->my_envp)[i++]);
		return (0);
	}
	i = 1;
	return (ft_export_loop(data, i, var_exists));
}
