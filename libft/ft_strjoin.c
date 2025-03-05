/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 14:21:23 by lshapkin          #+#    #+#             */
/*   Updated: 2024/04/30 16:12:23 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		len1;
	int		len2;
	char	*res;
	int		i;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	i = 0;
	res = (char *)malloc((len1 + len2 + 1) * sizeof(char));
	if (!res)
		return (NULL);
	while (i != (len1))
	{
		res[i] = s1[i];
		i++;
	}
	while (i != (len1 + len2))
	{
		res[i] = s2[i - len1];
		i++;
	}
	res[i] = '\0';
	return (res);
}
