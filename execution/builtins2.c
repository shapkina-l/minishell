/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 19:01:51 by lshapkin          #+#    #+#             */
/*   Updated: 2025/05/21 17:44:40 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	ft_export_var_create_p2(int len, t_data *data,
		char	*dup_var, char	**new_envp)
{
	int		insert_pos;
	int		i;

	insert_pos = 0;
	while (insert_pos < len && ft_strncmp(data->my_envp[insert_pos],
			dup_var, ft_strlen(data->my_envp[insert_pos]) + 1) < 0)
		insert_pos++;
	i = 0;
	while (i < insert_pos)
	{
		new_envp[i] = data->my_envp[i];
		i++;
	}
	new_envp[i] = dup_var;
	i++;
	while (i < len + 1)
	{
		new_envp[i] = data->my_envp[insert_pos];
		insert_pos++;
		i++;
	}
	new_envp[i] = NULL;
}

int	ft_export_var_create(char *new_var, t_data *data)
{
	int		len;
	char	**new_envp;
	char	*dup_var;

	if (!data->my_envp)
		return (-1);
	len = 0;
	while (data->my_envp && (data->my_envp)[len])
		len++;
	new_envp = malloc(sizeof(char *) * (len + 2));
	if (!new_envp)
		return (-1);
	dup_var = ft_strdup(new_var);
	if (!dup_var)
		return (free(new_envp), -1);
	ft_export_var_create_p2(len, data, dup_var, new_envp);
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
			return (print_error("export", "not a valid identifier"),
				free(var_name), 1);
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
		sort_envp(data->my_envp);
		while ((data->my_envp)[i])
			printf("declare -x %s\n", (data->my_envp)[i++]);
		return (0);
	}
	i = 1;
	return (ft_export_loop(data, i, var_exists));
}
