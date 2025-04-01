/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 16:04:52 by lshapkin          #+#    #+#             */
/*   Updated: 2024/08/09 13:06:54 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	count_words(char const *s, char c)
{
	int	counter;
	int	i;

	counter = 0;
	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] != c)
		{
			counter ++;
			while (s[i] != c && s[i] != '\0')
				i++;
		}
		else
			i++;
	}
	return (counter);
}

static int	word_len(char const *s, char c)
{
	int	i;

	i = 0;
	while (s[i] != '\0' && s[i] != c)
		i++;
	return (i);
}

static void	free_res(char **res)
{
	while (*res)
	{
		free(*res);
		res++;
	}
	free (res);
}

static char	**split(char const *s, char c, char**res, int counter)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < counter)
	{
		while (s[j] != '\0' && s[j] == c)
			j++;
		res[i] = ft_substr(s, j, word_len(&s[j], c));
		if (!res[i])
		{
			free_res(res);
			return (NULL);
		}
		while (s[j] != '\0' && s[j] != c)
			j++;
		i++;
	}
	return (res);
}

char	**ft_split(char const *s, char c)
{
	char	**res;
	int		words;

	if (!s)
		return (NULL);
	words = count_words(s, c);
	res = (char **)ft_calloc((words + 1), sizeof(char *));
	if (!res)
		return (NULL);
	res = split(s, c, res, words);
	return (res);
}
