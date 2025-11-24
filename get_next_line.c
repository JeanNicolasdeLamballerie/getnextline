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

#include <stdio.h>
#ifndef BUFFER_SIZE
# define BUFFER_SIZE 42
#endif
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_reader
{
	char			buffer[BUFFER_SIZE];
	char			*leftover;
	char			*line;
	int				ready;
	unsigned long	leftover_position;
	int				has_leftover_position;
	long			last_read;
}					t_reader;

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

t_reader	read_buffer(char *buf, t_reader reader, long bytes_read)
{
	long	index;
	long	leftover_size;
	char	*ptr;

	index = 0;
	// printf("\n[MARK0 ENTER] ptr %s \n", reader.leftover);
	// printf("\n[read_buffer] : On enter : %s | byte count %lu \n",
		// reader.leftover, bytes_read);
		while (index < bytes_read)
		{
			if (buf[index] == '\n')
			{
				// we have reached the end of a line. Return here.
				// We also need to put the leftover of the buffer in our leftover,
				// for next reads.
				if (reader.leftover)
				{
					// printf("\n[MARK1] ptr %s \n", reader.leftover);
					leftover_size = ft_strlen(reader.leftover);
					ptr = malloc(leftover_size + index + 2);
					// TODO: HANDLE ERROR
					if (!ptr)
						return (reader);
					ft_strlcpy(ptr, reader.leftover, leftover_size + 1);
					// printf("\n[read_buffer] : On copy : %s", ptr);
					free(reader.leftover);
					// printf("\n current buffer : #%s# \n", buf);
					reader.leftover = ft_substr(buf, 0, index + 2);
					// printf("\n Likely suspect : reader.leftover %s \n",
					// reader.leftover);
					ft_strlcat(ptr, reader.leftover, leftover_size + index + 2);
					// printf("\n Likely suspect : ptr %s \n", ptr);
					// printf("\n[read_buffer] : Concatenated : %s", ptr);
					free(reader.leftover);
					reader.line = ptr;
					reader.leftover = 0;
					reader.ready = 1;
					if (index + 1 != bytes_read)
					{
						reader.has_leftover_position = 1;
						reader.leftover_position = index + 1;
						// printf("\n|[PATH1 LEFTOVERS] -- idx %ld bytes read
						// % ld.|\n ", index+1,
						// bytes_read);
					}
				}
				else
				{
					reader.line = ft_substr(buf, 0, index + 1);
					reader.ready = 1;
					if (index + 1 != bytes_read)
					{
						reader.has_leftover_position = 1;
						reader.leftover_position = index + 1;
						// printf("\n[PATH2] idx %ld \n", index+1);
					}
				}
				return (reader);
			}
			index++;
		}
		// if the line wasnt contained in a single buffer,
		// then it spans across multiple ones.
		// In this case, we need to add the leftovers.
		if (!reader.ready)
		{
			if (reader.leftover)
			{
				// printf("\n[MARK2] ptr %s \n", reader.leftover);
				leftover_size = ft_strlen(reader.leftover);
				ptr = malloc(leftover_size + index + 1);
				// TODO: HANDLE ERROR
				if (!ptr)
					return (reader);
				ft_strlcpy(ptr, reader.leftover, leftover_size + 1);
				free(reader.leftover);
				reader.leftover = ft_substr(buf, 0, index + 1);
				ft_strlcat(ptr, reader.leftover, leftover_size + index + 1);
				free(reader.leftover);
				reader.leftover = ptr;
			} // now, the first instance of a fully leftover buffer :
			else
			{
				// If nothing is left in the buffer,
				// then there is nothing to extract.
				if (bytes_read)
				{
					reader.has_leftover_position = 1;
					reader.leftover_position = 0;
				}
			}
		}
		if (bytes_read == 0)
		{
			// we have reached the end of the last line.
			if (reader.leftover)
				reader.line = reader.leftover;
			reader.leftover = 0;
			reader.ready = 1;
		}
		return (reader);
}

char	*get_next_line(int fd)
{
	long			size;
	char			*line;
	static t_reader	reader;
	int				i;

	i = 0;
	if (fd < 0)
		return (0);
	while (!reader.ready)
	{
		if (reader.has_leftover_position)
		{
			// printf("\n[MARK :OUTSIDE] had ptr %s \n", reader.leftover);
			reader.leftover = ft_substr(reader.buffer, reader.leftover_position,
					reader.last_read - reader.leftover_position);
			reader.has_leftover_position = 0;
			while (reader.leftover[i])
			{
				if (reader.leftover[i] == '\n')
				{
					printf("");
					// reader.line = ft_substr(reader.leftover, 0, i+1); // or i+1
					// line = ft_substr(reader.leftover, i + 2, reader.last_read
					// 		- reader.leftover_position - i);
					// free(reader.leftover);
					// reader.leftover = line;
					// line = 0;
					// printf("\n  Found the bug  \n");
				}
				i++;
			}
			// printf("\n[MARK :OUTSIDE] ptr %s  (reader had leftover pos for
				// %lu)\n", reader.leftover, reader.leftover_position);
		}
		size = read(fd, reader.buffer, BUFFER_SIZE);
		if (size == -1)
			return (0);
		reader.last_read = size;
		reader = read_buffer(reader.buffer, reader, size);
	}
	reader.ready = 0;
	line = reader.line;
	reader.line = 0;
	return (line);
}
