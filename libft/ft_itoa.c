/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 16:19:25 by lshapkin          #+#    #+#             */
/*   Updated: 2024/04/30 17:15:58 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static unsigned int	abs_num_len(int n)
{
	unsigned int	len;
	unsigned int	num;

	len = 1;
	if (n < 0)
	{
		num = n * -1;
		len++;
	}
	else
		num = n;
	while (num >= 10)
	{
		num /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char			*res;
	int				len;
	int				i;
	unsigned int	num;
	int				j;

	i = 0;
	j = 1;
	len = abs_num_len(n);
	res = (char *)ft_calloc(len + 1, sizeof(char));
	if (!res)
		return (NULL);
	if (n < 0)
	{
		res[i++] = '-';
		num = -1 * n;
	}
	else
		num = n;
	while (len - j >= i)
	{
		res[len - j++] = ('0' + num % 10);
		num /= 10;
	}
	return (res);
}
