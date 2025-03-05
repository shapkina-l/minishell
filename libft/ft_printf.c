/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:18:44 by lshapkin          #+#    #+#             */
/*   Updated: 2024/08/07 21:52:33 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		counter;
	int		i;

	i = 0;
	counter = 0;
	if (format == NULL)
		return (-1);
	va_start(args, format);
	while (format[i] != '\0')
	{
		if (format[i] != '%')
		{
			counter += ft_putchar(format[i]);
			i++;
		}
		else
		{
			counter = call_function(format[i + 1], counter, args);
			i += 2;
		}
	}
	va_end(args);
	return (counter);
}

int	call_function(char conversion, int counter, va_list args)
{
	if (conversion == 'c')
		counter += ft_putchar(va_arg(args, int));
	if (conversion == 's')
		counter += ft_putstr(va_arg(args, char *));
	if (conversion == 'p')
		counter += ft_putptr(va_arg(args, long long unsigned int));
	if (conversion == 'd')
		counter += ft_putnbr(va_arg(args, int));
	if (conversion == 'i')
		counter += ft_putnbr(va_arg(args, int));
	if (conversion == 'u')
		counter += ft_putnbr_u(va_arg(args, unsigned int));
	if (conversion == 'x')
		counter += ft_puthex_low(va_arg(args, unsigned int));
	if (conversion == 'X')
		counter += ft_puthex_up(va_arg(args, unsigned int));
	if (conversion == '%')
		counter += ft_putchar('%');
	return (counter);
}
