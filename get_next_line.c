/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htran-th <htran-th@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 15:36:10 by htran-th          #+#    #+#             */
/*   Updated: 2024/06/07 18:31:49 by htran-th         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char *ft_free(char **s)
{
    if (*s)
    {
        free (*s);
        *s = NULL;
    }
    return (NULL);
}

static char *ft_readed(int fd, char *big_buf)
{
    int readed;
    char *small_buf;
    
    small_buf = NULL;
    small_buf = malloc((BUFFER_SIZE + 1) * sizeof(char));
    if(!small_buf)
        return (ft_free(&small_buf));
    readed = 0;
    while ((readed = read(fd, small_buf, BUFFER_SIZE)) > 0)
    {
        small_buf[readed] = '\0';

        big_buf = ft_strjoin_gnl(big_buf, small_buf);
        if(!big_buf)
            return (ft_free(&big_buf));
        if (ft_strchr(big_buf, '\n'))
            break;
    }
    if (readed == -1)
    {
        free (small_buf);
        return (ft_free(&big_buf));
    }
    free(small_buf);
    return (big_buf);
}

static char *ft_extract_readed_line(char *big_buf)
{
    char *readed_line;
    int i;
    
    i = 0;
    while (big_buf[i] != '\n' && big_buf[i] != '\0')
        i++;
    if (big_buf[i] == '\n')
        i++;
    readed_line = malloc((i + 1) * sizeof(char));
    if (!readed_line)
        return (ft_free(&readed_line));;
    ft_memcpy(readed_line, big_buf, i);
    readed_line[i] = '\0';
    return (readed_line);    
}
static void ft_get_remainder(char **big_buf)
{
    int i;
    //int len;
    char *new_big_buf;
    //int j;

    i = 0;
    while ((*big_buf)[i] != '\n' && (*big_buf)[i] != '\0')
        i++;
    if ((*big_buf)[i] == '\n')
        i++;
    /*if((*big_buf)[i] == '\0')//the EOF when there's no remainder
        return (ft_free(big_buf));*/
    new_big_buf = ft_strdup(*big_buf + i);
    free(*big_buf);
    *big_buf = new_big_buf;
    /*len = ft_strlen(*big_buf) - i;
    new_big_buf = malloc((len + 1) * sizeof(char));
    if (!new_big_buf)
        return (ft_free(&new_big_buf));
    j = 0;
    while ((*big_buf + i)[j] != '\0')
    {
        new_big_buf[j] = (*big_buf + i)[j];
        j++;
        i++;
    }
    new_big_buf[j] = '\0';
    ft_free(big_buf);
    return (new_big_buf);*/
}

char *get_next_line(int fd)
{
    static char *big_buf;
    char *line;
    
    line = NULL;
    if (fd < 0 ||BUFFER_SIZE <= 0 || read(fd, &line, 0) < 0)
    {
        ft_free (&big_buf);
        return (ft_free(&line));
    }
    big_buf = ft_readed(fd, big_buf);
    if (!big_buf || big_buf[0] == '\0')
        return (ft_free(&big_buf));
    line = ft_extract_readed_line(big_buf);
    if (!line)
        return (ft_free(&line));
    ft_get_remainder(&big_buf);
    return (line);    
}