/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 18:28:08 by lshapkin          #+#    #+#             */
/*   Updated: 2024/05/06 16:24:25 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	altc;
	char	*alts;
	int		i;

	altc = (char)c;
	alts = (char *)s;
	i = 0;
	while (alts[i] != '\0')
	{
		if (alts[i] == altc)
			return (&alts[i]);
		alts++;
	}
	if (alts[i] == altc)
		return (&alts[i]);
	return (NULL);
}
