/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 16:39:34 by lshapkin          #+#    #+#             */
/*   Updated: 2024/04/29 19:18:19 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	lendst;
	size_t	lensrc;
	size_t	i;

	lendst = ft_strlen(dst);
	lensrc = ft_strlen(src);
	if (size <= lendst)
		return (lensrc + size);
	i = 0;
	while (dst[i] != '\0' && i < size)
		i++;
	lendst = i;
	while (src[i - lendst] && i < size - 1)
	{
		dst[i] = src[i - lendst];
		i++;
	}
	if (lendst < size)
		dst[i] = '\0';
	return (lendst + lensrc);
}
