/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htran-th <htran-th@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 17:54:08 by htran-th          #+#    #+#             */
/*   Updated: 2024/06/11 21:14:50 by htran-th         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include "get_next_line.h"


int main(void)
{
    int fd;
    char *next_line;

    fd = open("example.txt", O_RDONLY);
    //fd = -1;
    while (1)
    {
        next_line = get_next_line(fd);
        if (!next_line)
            break ;
        printf("%s", next_line);
        free(next_line);
    }
    close(fd);
    return (0);
}