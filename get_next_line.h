/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htran-th <htran-th@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 16:02:35 by htran-th          #+#    #+#             */
/*   Updated: 2024/06/13 18:30:31 by htran-th         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

# include <stdlib.h>
# include <stddef.h>
# include <unistd.h>

char	*ft_strchr(const char *str, int c);
ssize_t	read(int fildes, void *buf, size_t nbyte);
char	*ft_strjoin_gnl(char *s1, char *s2);
char	*ft_strdup(char *s1);
size_t	ft_strlen(char *str);
void	*ft_memcpy(void *to, const void *from, size_t n);
char	*get_next_line(int fd);

#endif
