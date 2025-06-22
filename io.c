/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuwu <yuwu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 19:44:35 by yuwu              #+#    #+#             */
/*   Updated: 2025/06/22 19:03:13 by yuwu             ###   ########.fr       */
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

An exit code of 0 indicates success, a non-zero exit code indicates failure. 
The following failure codes can be returned:
    1   Error in command line syntax.
    2   One of the files passed on the command line did not exist.
    3   A required tool could not be found.
    4   The action failed.
Questions: are only negative nbrs treated as error and the rest as success
 
exit: cause the shell to exit-------------exit [n]
*/

#include "pipex.h"

int	open_infile(const char *filename)
{
	int	fd;

	if (!filename)
		return (-1);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (-1);
	return (fd);
}

//If it doesn’t exist, create it with correct permissions (e.g., 0644).
//If the file exists, truncate it (overwrite).
int	open_outfile(const char *filename)
{
	int	fd;

	if (!filename)
		return (-1);
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		return (-1);
	return (fd);
}

//send error msg and exit
void	ft_error(char *error_msg)
{
	write(2, "pipex: ", 7);
	write(2, error_msg, ft_strlen(error_msg));
	write(2, "\n", 1);
	exit(EXIT_FAILURE);
}

//close fds and pipefd
void	close_all_four(int fd1, int fd2, int ppfd[2])
{
	close(fd1);
	close(fd2);
	close(ppfd[0]);
	close(ppfd[1]);
}