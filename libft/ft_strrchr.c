/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 19:10:17 by lshapkin          #+#    #+#             */
/*   Updated: 2024/05/06 16:28:16 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	altc;
	char	*alts;
	int		i;
	char	*res;

	altc = (char)c;
	alts = (char *)s;
	i = 0;
	res = NULL;
	while (alts[i] != '\0')
	{
		if (alts[i] == altc)
			res = &alts[i];
		alts++;
	}
	if (alts[i] == altc)
		res = &alts[i];
	return (res);
}
