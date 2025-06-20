/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuwu <yuwu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 17:27:50 by yuwu              #+#    #+#             */
/*   Updated: 2025/06/19 17:28:23 by yuwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
----------------------examples of how pipe works-------------------------------
./pipex infile "grep hello" "wc -l" outfile
- pipex: program name (av[0])
- infile: read the inout file
- "grep hello": find lines with hello inside
- wc: word count
- outfile: printf how many hello lines r there

-------------------------------------
for the cmd: I dont need to hard code it, they are ready from shell
-------------------------------------

*/

#ifndef PIPEX_H
# define PIPEX_H

# include<unistd.h>  //write, open, access, dup, dup2, execve, fork, pipe
# include<stdlib.h>  //malloc, free, exit
# include<fcntl.h>  //open
# include<stdio.h>  //write, perror
# include<string.h> //strerror

//io.c: input and output
int open_infile(const char *filename);
int open_outfile(const char *filename);

//cmd.c get and execute cmds
char **make_cmd_array(char *command_str);   //find the command
char *find_command_in_path(char *cmd, char *path); //find the path
void exe_cmd(input, cmd, output);   //execute the cmd from the path

//main.c: main process, excution function
char *find_path_in_envp(char **envp); //find the environmental pointer
int main(int ac, char **av, char **envp);

#endif
