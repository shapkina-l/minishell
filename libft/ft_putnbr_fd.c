/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 17:53:26 by lshapkin          #+#    #+#             */
/*   Updated: 2024/05/03 17:59:27 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	char			a;
	unsigned int	nbr;

	if (n < 0)
	{
		write(fd, "-", 1);
		nbr = n * -1;
	}
	else
		nbr = n;
	if (nbr < 10)
	{
		a = nbr + '0';
		write(fd, &a, 1);
	}
	else
	{
		ft_putnbr_fd(nbr / 10, fd);
		a = nbr % 10 + '0';
		write(fd, &a, 1);
	}
}
