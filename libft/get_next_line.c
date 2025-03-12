/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 20:02:52 by lshapkin          #+#    #+#             */
/*   Updated: 2024/08/09 14:54:09 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*get_next_line(int fd)
{
	char		*line;
	static char	left_line[BUFFER_SIZE + 1];
	char		*buffer;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, left_line, 0) < 0)
	{
		ft_bzero (left_line, BUFFER_SIZE + 1);
		return (NULL);
	}
	buffer = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (NULL);
	ft_bzero(buffer, (BUFFER_SIZE + 1) * sizeof(char));
	if (left_line[0] != '\0')
		ft_strlcpy(buffer, left_line, ft_strlen(left_line) + 1);
	line = read_and_fill_buffer(fd, left_line, buffer);
	if (!line || (line[0] == '\0' && left_line[0] == '\0'))
	{
		free(line);
		return (NULL);
	}
	return (line);
}

char	*read_and_fill_buffer(int fd, char *left_line, char *buffer)
{
	int		check;

	check = 1;
	while (check > 0)
	{
		if (handle_newline(left_line, buffer))
			return (buffer);
		check = read(fd, left_line, BUFFER_SIZE);
		if (check < 0)
			return (free(buffer), update_left_line(left_line), NULL);
		if (check == 0)
		{
			update_left_line(left_line);
			break ;
		}
		else
		{
			left_line[check] = '\0';
			buffer = ft_append(buffer, left_line);
			if (handle_newline(left_line, buffer))
				break ;
		}
	}
	return (buffer);
}

int	handle_newline(char *left_line, char *buffer)
{
	char	*newline_pos;
	char	*i;

	newline_pos = ft_strchr(left_line, '\n');
	if (newline_pos != NULL)
	{
		i = ft_strchr(buffer, '\n');
		ft_bzero(i + 1, ft_strlen(i));
		update_left_line(left_line);
		return (1);
	}
	return (0);
}

void	update_left_line(char *left_line)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (left_line[i] != '\0' && left_line[i] != '\n')
		i++;
	if (left_line[i] == '\0')
	{
		ft_bzero(left_line, i + 1);
		return ;
	}
	while (left_line[i + j])
	{
		left_line[j] = left_line[i + j + 1];
		j++;
	}
	left_line[j] = '\0';
}
