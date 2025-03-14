/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 19:01:51 by lshapkin          #+#    #+#             */
/*   Updated: 2025/03/14 17:28:16 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int ft_echo(t_data *data)
{
    int i = 1;
    int newline = 1;

    if (data->args[1] && ft_strcmp(data->args[1], "-n") == 0)
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
    return 0;
}

int ft_cd(t_data *data)
{
    char *path;
    
    if (data->args[1])
        path = data->args[1];
    else
        path = getenv("HOME");
    if (!path)
    {
        fprintf(stderr, "cd: HOME not set\n");
        return 1;
    }
    if (chdir(path) != 0)
    {
        perror("cd");
        return 1;
    }
    return 0;
}

int ft_pwd()
{
    char cwd[PATH_MAX];
    
    if (getcwd(cwd, sizeof(cwd)))
        printf("%s\n", cwd);
    else
        perror("pwd");
    return 0;
}

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
ft_export_var_create(char *new_var, char *envp[])
{
    int len;
    
    len = 0;
    while (envp[len]) 
        len++;
    envp = realloc(envp, sizeof(char *) * (len + 2));
    envp[len] = new_var;
    envp[len + 1] = NULL;
}

int ft_export(t_data *data, char *envp[])
{
    int i;
    int var_exists;

    if (!data->args[1])
    {
        i = 0;
        while(envp[i])
        {
            printf("declare -x %s\n", envp[i]);
            i++;
        }
        return 0;
    }
    i = 1;
    // Loop through each argument after "export"
    while(data->args[i]) 
    {
        var_exists = ft_export_var_exists(data->args[i], envp);
        if (!var_exists) 
            ft_export_var_create(data->args[i], envp);
        i++;
    }
    return 0;
}

int ft_unset(t_data *data, char *envp[]) 
{
    int i;
    int j;
    
    if (!data->args[1]) {
        fprintf(stderr, "unset: not enough arguments\n");
        return 1;
    }
    i = 1;
    while (data->args[i])
    {
        j = 0;
        while (envp[j]) 
        {
            // Find the variable in envp (before '=' sign)
            if (strncmp(envp[j], data->args[i], strlen(data->args[i])) == 0 &&
                envp[j][strlen(data->args[i])] == '=') 
            {
                free(envp[j]); // Free the variable
                // Shift elements up
                while (envp[j]) 
                {
                    envp[j] = envp[j + 1];
                    j++;
                }
                break ; // Stop searching after removing the first occurrence
            }
            j++;
        }
        i++;
    }
    return 0;
}

int ft_env(char *envp[])
{
    int i;

    i = 0;
    while (envp[i])
    {
        printf("%s\n", envp[i]);
        i++;
    }
    return 0;
}

int ft_exit()
{
    printf("exit\n");
    exit(0);
}