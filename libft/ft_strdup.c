/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 17:49:58 by lshapkin          #+#    #+#             */
/*   Updated: 2024/04/29 14:55:58 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*res;
	int		len;
	int		i;

	len = 0;
	i = 0;
	while (s[len] != '\0')
		len++;
	res = (char *)malloc((len + 1) * sizeof(char));
	if (!res)
		return (NULL);
	while (i != len)
	{
		res[i] = s[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}
