/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenicola <jean.nicolas-de-lamballerie@lea  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 14:46:22 by jenicola          #+#    #+#             */
/*   Updated: 2025/11/25 17:55:19 by jenicola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

char	*process_leftovers(long index, long size_marker, t_reader *reader)
{
	long	leftover_size;
	char	*ptr;

	leftover_size = ft_strlen(reader->leftover);
	ptr = malloc(leftover_size + index + size_marker);
	if (!ptr)
		return (ptr);
	ft_strlcpy(ptr, reader->leftover, leftover_size + 1);
	free(reader->leftover);
	reader->leftover = ft_substr(reader->buffer, 0, index + size_marker);
	ft_strlcat(ptr, reader->leftover, leftover_size + index + size_marker);
	free(reader->leftover);
	return (ptr);
}

static t_error	process_newline(long index, long bytes_read, char *ptr,
		t_reader *reader)
{
	if (reader->leftover)
	{
		ptr = process_leftovers(index, 2, reader);
		if (!ptr)
			return (1);
		reader->line = ptr;
		reader->leftover = 0;
		reader->ready = 1;
		if (index + 1 != bytes_read)
		{
			reader->has_leftover_position = 1;
			reader->leftover_position = index + 1;
		}
	}
	else
	{
		reader->line = ft_substr(reader->buffer, 0, index + 1);
		reader->ready = 1;
		if (index + 1 != bytes_read)
		{
			reader->has_leftover_position = 1;
			reader->leftover_position = index + 1;
		}
	}
	return (0);
}

static t_error	read_buffer(char *buf, t_reader *reader, long bytes_read)
{
	long	index;
	char	*ptr;

	index = 0;
	ptr = 0;
	while (index < bytes_read)
	{
		if (buf[index] == '\n')
		{
			return (process_newline(index, bytes_read, ptr, reader));
		}
		index++;
	}
	cleanup_post_read(index, reader, ptr, bytes_read);
	return (0);
}

static char	*process_possible_line(t_reader *reader, char *line)
{
	int	i;

	i = 0;
	line = 0;
	while (reader->leftover && reader->leftover[i])
	{
		if (reader->leftover[i] == '\n')
		{
			reader->line = ft_substr(reader->leftover, 0, i + 1);
			line = ft_substr(reader->leftover, i + 1, reader->last_read
					- reader->leftover_position - i);
			if (ft_strlen(line) == 0)
			{
				free(line);
				line = 0;
			}
			free(reader->leftover);
			reader->leftover = line;
			line = reader->line;
			reader->line = 0;
			break ;
		}
		i++;
	}
	return (line);
}

char	*get_next_line(int fd)
{
	char			*line;
	static t_reader	reader;

	if (fd < 0)
		return (0);
	while (!reader.ready)
	{
		if (reader.has_leftover_position)
		{
			reader.leftover = ft_substr(reader.buffer, reader.leftover_position,
					reader.last_read - reader.leftover_position);
			reader.has_leftover_position = 0;
		}
		line = process_possible_line(&reader, line);
		if (line)
			return (line);
		reader.last_read = read(fd, reader.buffer, BUFFER_SIZE);
		if (reader.last_read == -1 || read_buffer(reader.buffer, &reader,
				reader.last_read))
			return (0);
	}
	reader.ready = 0;
	line = reader.line;
	reader.line = 0;
	return (line);
}
