/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 19:01:51 by lshapkin          #+#    #+#             */
/*   Updated: 2025/03/16 11:17:19 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int ft_export_var_exists(char *new_var, char *envp[])
{
    int     j;
    // Check if the variable already exists in the environment
    j = 0;
    while (envp[j]) 
    {
        if (ft_strncmp(envp[j], new_var, strchr(new_var, '=') - new_var) == 0) 
        {
        // Modify the environment variable
            envp[j] = new_var;  // Directly modify the value
            return (1);
        }
        j++;
    }
    return (0);
}

// If the variable doesn't exist, create a new one
void ft_export_var_create(char *new_var, char ***envp)
{
    int len;
    
    len = 0;
    while ((*envp)[len]) 
        len++;
    char **new_envp = ft_realloc(*envp, len + 1, sizeof(char *) * (len + 2));
    if (!new_envp)
        return ;
    new_envp[len] = new_var;
    new_envp[len + 1] = NULL;
    *envp = new_envp;
    free (new_envp);
}

int ft_export(t_data *data, char ***envp)
{
    int i;
    int var_exists;

    if (!data->args[1])
    {
        i = 0;
        while(envp[i])
        {
            printf("declare -x %s\n", (*envp)[i]);
            i++;
        }
        return (0);
    }
    i = 1;
    // Loop through each argument after "export"
    while(data->args[i]) 
    {
        var_exists = ft_export_var_exists(data->args[i], *envp);
        if (!var_exists) 
            ft_export_var_create(data->args[i], envp);
        i++;
    }
    return (0);
}

int ft_unset(t_data *data, char ***envp) 
{
    int i;
    int j;
    
    if (!data->args[1])
    {
        printf("unset: not enough arguments\n");
        return (1);
    }
    i = 1;
    while (data->args[i])
    {
        j = 0;
        while ((*envp)[j]) 
        {
            // Find the variable in envp (before '=' sign)
            if (strncmp((*envp)[j], data->args[i], strlen(data->args[i])) == 0 &&
                (*envp)[j][strlen(data->args[i])] == '=') 
            {
                free((*envp)[j]); // Free the variable
                // Shift elements up
                while ((*envp)[j]) 
                {
                    (*envp)[j] = (*envp)[j + 1];
                    j++;
                }
                break ; // Stop searching after removing the first occurrence
            }
            j++;
        }
        i++;
    }
    return (0);
}
