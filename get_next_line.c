/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenicola <jean.nicolas-de-lamballerie@lea  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 14:46:22 by jenicola          #+#    #+#             */
/*   Updated: 2025/11/25 16:10:10 by jenicola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

t_reader	read_buffer(char *buf, t_reader reader, long bytes_read)
{
	long	index;
	long	leftover_size;
	char	*ptr;

	index = 0;
	while (index < bytes_read)
	{
		if (buf[index] == '\n')
		{
			// we have reached the end of a line. Return here.
			// We also need to put the leftover of the buffer in our leftover,
			// for next reads.
			if (reader.leftover)
			{
				leftover_size = ft_strlen(reader.leftover);
				ptr = malloc(leftover_size + index + 2);
				// TODO: HANDLE ERROR
				if (!ptr)
					return (reader);
				ft_strlcpy(ptr, reader.leftover, leftover_size + 1);
				free(reader.leftover);
				reader.leftover = ft_substr(buf, 0, index + 2);
				ft_strlcat(ptr, reader.leftover, leftover_size + index + 2);
				free(reader.leftover);
				reader.line = ptr;
				reader.leftover = 0;
				reader.ready = 1;
				if (index + 1 != bytes_read)
				{
					reader.has_leftover_position = 1;
					reader.leftover_position = index + 1;
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
		i = 0;
		while (reader.leftover && reader.leftover[i])
		{
			if (reader.leftover[i] == '\n')
			{
				reader.line = ft_substr(reader.leftover, 0, i + 1); // or i+1
				line = ft_substr(reader.leftover, i + 1, reader.last_read
						- reader.leftover_position - i);
				if (ft_strlen(line) == 0)
				{
					free(line);
					line = 0;
				}
				free(reader.leftover);
				reader.leftover = line;
				line = reader.line;
				reader.line = 0;
				return (line);
			}
			i++;
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
