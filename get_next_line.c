/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htran-th <htran-th@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 15:36:10 by htran-th          #+#    #+#             */
/*   Updated: 2024/06/05 14:59:29 by htran-th         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char *ft_readed(int fd, char *big_buf)
{
    int readed;
    char *small_buf;
    
    small_buf = NULL;
    small_buf = malloc((BUFFER_SIZE + 1) * sizeof(char));
    if(!small_buf)
        return (NULL);
    readed = 1;
    //at this point, the address of small_buf changes after each iteration,
    //the free (small_buf) later won't free the exact memory block, causing
    //memory leak
    while ((*small_buf++ != '\n' && *small_buf++ != '\0') && readed)
    {
        readed = read(fd, small_buf, BUFFER_SIZE);
        if (readed == -1)
        {
            free(small_buf);
            return (NULL);
        }
        small_buf[readed] = '\0';
        //big_buf address changes after each concatenation & the previous
        //address of big_buf is lost -> won't be freed. A char *temp is needed
        //to hold the previous address of big_buf right before each concatenation.
        //free(temp) right after updating big_buf (with ft_strjoin) will free the
        //previous address of big_buf.
        big_buf = ft_strjoin(big_buf, small_buf);
    }
    free(small_buf);
    return (big_buf);
}

static char *ft_extract_readed_line(char *big_buf)
{
    char *readed_line;
    int i;
    
    i = 0;
    while (big_buf[i] != '\n' && big_buf != '\0')
    {
        i++;
    }
    if (big_buf[i] == '\n')
        i++;
    readed_line = malloc((i + 1) * sizeof(char));
    if (!readed_line)
        return (NULL);
    ft_memcpy(readed_line, big_buf, i);
    readed_line[i] = '\0';
    return (readed_line);    
}
static char *ft_get_remainder(char *big_buf)
{
    int i;
    static char *new_big_buf;

    i = 0;
    while (big_buf[i] != '\n')
    {
        i++;
    }
    if (big_buf[i] == '\n')
    i++;
    new_big_buf = ft_strdup(&big_buf[i]);
    free(big_buf);
    big_buf = NULL;
    return (new_big_buf);
}

char *get_next_line(int fd)
{
    static char *big_buf;
    static char *line;
    
    if (fd < 0 ||BUFFER_SIZE <= 0 || read(fd, &line, 0) < 0)
        return (NULL);
    big_buf = ft_readed(fd, big_buf);// put whatever in small buffer to big buffer, maybe use strjoin?
    line = ft_extract_readed_line(big_buf); // put whatever in the big buffer to line until '\n', str concatenate?
    big_buf = ft_get_remainder(big_buf);// put the remainder into a new string, free the old big_buf,
                                        // set the new string to be the big_buf
    return (line);    
}
