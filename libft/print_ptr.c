/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ptr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:19:11 by lshapkin          #+#    #+#             */
/*   Updated: 2024/08/07 21:52:58 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_num_len_ptr(unsigned long long nb)
{
	int	len;

	len = 0;
	if (nb >= 16)
	{
		len += ft_num_len_ptr(nb / 16);
		len += ft_num_len_ptr(nb % 16);
	}
	else
		len++;
	return (len);
}

int	ft_putptr(unsigned long long nb)
{
	unsigned long long	nbr;
	char				*str;
	int					i;

	if (nb == 0)
		return (ft_putstr("(nil)"));
	nbr = nb;
	i = (ft_num_len_ptr(nb));
	str = malloc((i + 1) * sizeof(char));
	if (!str)
		return (0);
	str[i] = '\0';
	i--;
	while (nb != 0)
	{
		if ((nb % 16) < 10)
			str[i--] = nb % 16 + '0';
		else
			str[i--] = nb % 16 + 87;
		nb /= 16;
	}
	ft_putstr("0x");
	ft_putstr(str);
	free(str);
	return (ft_num_len_ptr(nbr) + 2);
}
