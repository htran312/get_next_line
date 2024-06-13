/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htran-th <htran-th@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 15:36:10 by htran-th          #+#    #+#             */
/*   Updated: 2024/06/12 13:00:06 by htran-th         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*ft_free(char **s)
{
    //instead of only returning NULL, this function not only free the memory
    //block, but also set the pointer to NULL. This ensure that free(*s) is
    //only called when *s id a valid pointer that was previously allocated
    //memory.
	if (*s)
	{
		free (*s);//releases the allocated memory
		*s = NULL;//sets the pointer to NULL to prevent dangling pointer
	}
	return (NULL);
}

static char	*ft_read(int fd, char *big_buf, int been_read, char *small_buf)
{
	char	*tmp;

    //the condition of the loop is (1), it breaks if it's (0). It won't break
    //if read return (-1) unless specified.
	while (been_read)
	{
		been_read = read(fd, small_buf, BUFFER_SIZE);
		if (been_read < 0)
		{
			free (small_buf);
			return (ft_free(&big_buf));
		}
		small_buf[been_read] = '\0';
        //set tmp to also points to big_buf BEFORE the join
		tmp = big_buf;
        //big_buf's updated
		big_buf = ft_strjoin_gnl(big_buf, small_buf);
        //if ft_strjoin fails -> the new big_buf fails to update. We need
        //to free small_buf & the previous big_buf (aka tmp, because the
        //new big_buf has failed, so we don't need to free that)
		if (!big_buf)
		{
			free (small_buf);
			return (ft_free(&tmp));
		}
        //stops reading when encounters '\n' & breaks the loop
		if (ft_strchr(big_buf, '\n'))
			break ;
	}
	//after looping, free small_buf because we malloc memory for it
	free(small_buf);
	return (big_buf);
}
//this function is to extract the line until a newline character from big_buf
static char	*ft_extract_read_line(char *big_buf)
{
	char	*read_line;
	int		i;

	//the idea is to loop through big_buf until '\n' or '\0' (at EOF)
	i = 0;
	if (!big_buf[i])
		return (NULL);
	while (big_buf[i] != '\n' && big_buf[i] != '\0')
		i++;
	//if encounter a '\n' character, move forward because we need to extract
	//that '\n' too
	if (big_buf[i] == '\n')
		i++;
	//malloc (i + 1) because we need space for the '\0'
	read_line = malloc((i + 1) * sizeof(char));
	if (!read_line)
		return (NULL);
	ft_memcpy(read_line, big_buf, i);
	read_line[i] = '\0';
	return (read_line);
}

static void	ft_get_remainder(char **big_buf)
{
	int		i;
	char	*new_big_buf;

	i = 0;
    // Find the end of the current line
	while ((*big_buf)[i] != '\n' && (*big_buf)[i] != '\0')
		i++;
	if ((*big_buf)[i] == '\n')
		i++;
	/*at the EOF meaning there's no remainder, we still need to free
    (the whole) big_buf. Check if at the current index, if big_buf is
    null (meaning after '\n' there's nothing) -> do the below*/
	if (!(*big_buf + i))
	{
		ft_free(big_buf);
		return ;
	}
    // Create a new buffer starting from the next line
	new_big_buf = ft_strdup(*big_buf + i);
    // Free the old buffer
	free(*big_buf);
    // Update the original pointer to point to the new buffer
	*big_buf = new_big_buf;
}

char	*get_next_line(int fd)
{
	static char	*big_buf;
	char		*small_buf;
	char		*line;
	int			been_read;

	line = NULL;
	//check if fd is valid (can't be negative), for bonus, (fd < some limit)
	//BUFFER_SIZE <=0 means nothing to read/read error
	//if read function return negative value (should be -1), then error happened
	//try read from fd but not putting the read value to any buffer (put NULL)
	//and read 0 bytes (it's just a check)
	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, NULL, 0) < 0)
		return (ft_free(&big_buf));
	//set this value to be 1, and this'll be the condition of the while loop
	//for read, meaning as long as this's true, it keeps looping. It will no longer
	//be true only if it's 0, aka EOF (read: Truthy and Falsey Vaues). So even if
	//it's (-1), it's still satisfied the loop condition. But (-1) meaning read error
	//so we need a check for that.
	been_read = 1;
	//malloc BUFFER_SIZE + 1 because the '\0'
	small_buf = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!small_buf)
		return (ft_free(&big_buf));
	//big_buf after ft_read executes: carries a null-terminated string that CONTAINS
	//'\n' and the remainder (null if EOF)
	big_buf = ft_read(fd, big_buf, been_read, small_buf);
	//check if the process failed, doesn't need to use ft_free because
	//big_buf is already null in this case, right? Just return (NULL) as standard
    if (!big_buf)
		return (NULL);
	//line after ft_extract_line: carries the line that's newline-terminated that
	//had been extracted from big_buf without the remainder (ofc!)
	line = ft_extract_read_line(big_buf);
	//if the process failed, big_buf needs to be freed because even the extraction
	//failed, there're still content in big_buf
	if (!line)
		return (ft_free(&big_buf));
	//this void function will extract the remainder from big_buf, put it into a
	//new_big_buf, free the (old) big_buf, and set the 
	ft_get_remainder(&big_buf);
	return (line);
}
