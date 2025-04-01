/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_hex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:19:07 by lshapkin          #+#    #+#             */
/*   Updated: 2024/08/07 21:52:58 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_num_len(unsigned int nb)
{
	int	len;

	len = 0;
	if (nb == 0)
		len = 1;
	while (nb != 0)
	{
		len++;
		nb /= 16;
	}
	return (len);
}

void	ft_printhex_low(unsigned int nb)
{
	static char	lower_digits[] = "0123456789abcdef";

	if (nb >= 16)
		ft_printhex_low(nb / 16);
	ft_putchar(lower_digits[nb % 16]);
}

int	ft_puthex_low(unsigned int nb)
{
	ft_printhex_low(nb);
	return (ft_num_len(nb));
}

void	ft_printhex_up(unsigned int nb)
{
	static char	upper_digits[] = "0123456789ABCDEF";

	if (nb >= 16)
		ft_printhex_up(nb / 16);
	ft_putchar(upper_digits[nb % 16]);
}

int	ft_puthex_up(unsigned int nb)
{
	ft_printhex_up(nb);
	return (ft_num_len(nb));
}
