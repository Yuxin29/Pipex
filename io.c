/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuwu <yuwu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 19:44:35 by yuwu              #+#    #+#             */
/*   Updated: 2025/06/19 19:45:04 by yuwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
─ open_infile() (parent)
Tries to open the input file in read-only mode.
On success: returns the file descriptor.
On failure: prints an error and exits the program

─ open_outfile() (parent)
Tries to open or create the output file in write mode.
On success: returns the file descriptor.
On failure: prints an error and exits. 

An exit code of 0 indicates success while a non-zero exit code indicates failure. 
The following failure codes can be returned:
    1   Error in command line syntax.
    2   One of the files passed on the command line did not exist.
    3   A required tool could not be found.
    4   The action failed.
Questions: are only negative nbrs treated as error and the rest as success
 
exit: cause the shell to exit-------------exit [n]

    */

#include "pipex.h"

int open_infile(const char *filename)
{
    int fd;

    fd = open(filename, 0_RDONLY);
    if (fd != 0)
    {
        //return (ERROR);
        perror("Error openning infile");
        exit(EXIT_FAILURE);
    }
    return (fd);
}

int open_outfile(const char *filename)
{
    int fd;

    //If it doesn’t exist, create it with correct permissions (e.g., 0644).
    //If the file exists, truncate it (overwrite).
    fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd != 0)
    {
        //return (ERROR);
        perror("Error openning infile");
        exit(EXIT_FAILURE);
    }
    return (fd);
}