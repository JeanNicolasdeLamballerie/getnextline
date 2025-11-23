/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenicola <jean.nicolas-de-lamballerie@lea  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 14:46:22 by jenicola          #+#    #+#             */
/*   Updated: 2025/11/23 17:59:20 by jenicola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 42
#endif
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_reader
{
	char	*leftover;
	char	*line;
	int		ready;
}			t_reader;

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	while (src[i] && i < size - 1 && size != 0)
	{
		dst[i] = src[i];
		i++;
	}
	if (size > 0)
		dst[i] = '\0';
	while (src[i])
		i++;
	return (i);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	min_len;
	char	*ptr;
	size_t	l;

	if (!s)
		return (0);
	min_len = 0;
	l = ft_strlen(s);
	if (l < start)
	{
		ptr = malloc(1);
		if (!ptr)
			return (ptr);
		ptr[0] = 0;
		return (ptr);
	}
	while ((s + start)[min_len] && min_len < len)
	{
		min_len++;
	}
	ptr = malloc(min_len + 1);
	if (!ptr)
		return (ptr);
	ft_strlcpy(ptr, s + start, min_len + 1);
	return (ptr);
}


size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	pos;

	pos = 0;
	if (!size && (!dst || !src))
		return (0);
	while (dst[pos] && pos < size)
		pos++;
	while (*src && pos + 1 < size)
	{
		dst[pos] = *src;
		src++;
		pos++;
	}
	if (size > 0 && pos < size)
		dst[pos] = '\0';
	while (*src++)
		pos++;
	return (pos);
}

t_reader	read_buffer(char *buf, char *source, long size)
{
	t_reader	result;
	long		index;

	index = 0;
	result.ready = 0;
	while (index < size)
	{
		if (buf[index] == '\n')
		{
			result.line = ft_substr(buf, 0, index+1);
			if (source) {

			}
				
			break;
		}
		index++;
	}
	return (result);
}

char	*get_next_line(int fd)
{
	static char buffer[BUFFER_SIZE];
	long size;
	t_reader reader;
	reader.ready = 0;
	reader.leftover = 0;
	while (!reader.ready)
	{
		size = read(fd, buffer, BUFFER_SIZE);
		reader = read_buffer(buffer, reader.leftover, size);
	}
	return (reader.line);
}

// typedef struct s_reader
// {
// 	char	*line;
// 	int		ready;
// }			t_reader;
//
