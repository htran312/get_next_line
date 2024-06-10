/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htran-th <htran-th@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 15:36:10 by htran-th          #+#    #+#             */
/*   Updated: 2024/06/10 20:10:36 by htran-th         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char *ft_free(char **s)
{
    if (s && *s)
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
    char *tmp;
    
    small_buf = malloc((BUFFER_SIZE + 1) * sizeof(char));
    if(!small_buf)
        return (ft_free(&big_buf));
    readed = 0;
    while ((readed = read(fd, small_buf, BUFFER_SIZE)) > 0)
    {
        small_buf[readed] = '\0';
        tmp = big_buf;
        big_buf = ft_strjoin_gnl(big_buf, small_buf);
        if(!big_buf)
        {
            free (small_buf);
            return (ft_free(&tmp));
        }
        if (ft_strchr(big_buf, '\n'))
            break;
    }
    if (readed < 0)
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
    if (!big_buf[i])
        return (NULL);
    while (big_buf[i] != '\n' && big_buf[i] != '\0')
        i++;
    if (big_buf[i] == '\n')
        i++;
    readed_line = malloc((i + 1) * sizeof(char));
    if (!readed_line)
        return (NULL);
    ft_memcpy(readed_line, big_buf, i);
    readed_line[i] = '\0';
    return (readed_line);    
}
static void ft_get_remainder(char **big_buf)
{
    int i;
    char *new_big_buf;

    i = 0;
    while ((*big_buf)[i] != '\n' && (*big_buf)[i] != '\0')
        i++;
    if ((*big_buf)[i] == '\n')
        i++;
    if (!(*big_buf + i))
    {
        ft_free(big_buf);
        return ;
    }
    /*if((*big_buf)[i] == '\0')//the EOF when there's no remainder
        return (ft_free(big_buf));*/
    new_big_buf = ft_strdup(*big_buf + i);
    free(*big_buf);
    *big_buf = new_big_buf;
}

char *get_next_line(int fd)
{
    static char *big_buf;
    char *line;
    
    line = NULL;
    if (fd < 0 ||BUFFER_SIZE <= 0 || read(fd, NULL, 0) < 0)
    {
        free(big_buf);
        big_buf = NULL;
        return (NULL);
    }
    big_buf = ft_readed(fd, big_buf);
    if (!big_buf)
        return (NULL);
    line = ft_extract_readed_line(big_buf);
    if (!line)
        return (ft_free(&big_buf));
    ft_get_remainder(&big_buf);
    return (line);    
}
