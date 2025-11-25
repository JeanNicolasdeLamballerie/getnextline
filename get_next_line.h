/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenicola <jean.nicolas-de-lamballerie@lea  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 14:44:30 by jenicola          #+#    #+#             */
/*   Updated: 2025/11/25 17:55:03 by jenicola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# include <stddef.h>
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

typedef int			t_error;

typedef struct s_reader
{
	char			buffer[BUFFER_SIZE];
	char			*leftover;
	char			*line;
	int				ready;
	unsigned long	leftover_position;
	int				has_leftover_position;
	long			last_read;
	int				error;
}					t_reader;

char				*process_leftovers(long index, long size_marker,
						t_reader *reader);

void				cleanup_post_read(long index, t_reader *reader, char *ptr,
						long bytes_read);
size_t				ft_strlen(const char *s);
size_t				ft_strlcpy(char *dst, const char *src, size_t size);
size_t				ft_strlcat(char *dst, const char *src, size_t size);
char				*ft_substr(char const *s, unsigned int start, size_t len);
char				*get_next_line(int fd);
#endif
