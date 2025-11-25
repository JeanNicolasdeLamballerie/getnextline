/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenicola <jean.nicolas-de-lamballerie@lea  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 16:07:59 by jenicola          #+#    #+#             */
/*   Updated: 2025/11/25 17:55:03 by jenicola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stddef.h>
#include <stdlib.h>

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

void	cleanup_post_read(long index, t_reader *reader, char *ptr,
		long bytes_read)
{
	if (!reader->ready)
	{
		if (reader->leftover)
		{
			ptr = process_leftovers(index, 1, reader);
			reader->leftover = ptr;
		}
		else
		{
			if (bytes_read)
			{
				reader->has_leftover_position = 1;
				reader->leftover_position = 0;
			}
		}
	}
	if (bytes_read == 0)
	{
		if (reader->leftover)
			reader->line = reader->leftover;
		reader->leftover = 0;
		reader->ready = 1;
	}
}
