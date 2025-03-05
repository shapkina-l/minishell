/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_nbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:19:15 by lshapkin          #+#    #+#             */
/*   Updated: 2024/08/07 21:52:58 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putnbr(int nb)
{
	unsigned int	nbr;
	unsigned int	nbrcpy;
	int				counter;

	counter = 0;
	if (nb < 0)
	{
		ft_putchar('-');
		nbr = nb * -1;
		counter++;
	}
	else
		nbr = nb;
	nbrcpy = nbr;
	if (nbrcpy == 0)
		counter = 1;
	while (nbrcpy != 0)
	{
		counter++;
		nbrcpy /= 10;
	}
	if (nbr >= 10)
		ft_putnbr(nbr / 10);
	ft_putchar(nbr % 10 + 48);
	return (counter);
}

int	ft_putnbr_u(unsigned int nb)
{
	unsigned int	nbr;
	unsigned int	nbrcpy;
	int				counter;

	counter = 0;
	nbr = nb;
	nbrcpy = nbr;
	if (nbrcpy == 0)
		counter = 1;
	while (nbrcpy != 0)
	{
		counter++;
		nbrcpy /= 10;
	}
	if (nbr >= 10)
		ft_putnbr(nbr / 10);
	ft_putchar(nbr % 10 + 48);
	return (counter);
}
