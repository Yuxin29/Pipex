/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuwu <yuwu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 19:46:51 by yuwu              #+#    #+#             */
/*   Updated: 2025/06/19 19:46:56 by yuwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//get and execute cmds
//for the cmd: I dont need to hard code it, they are ready from shell

// Split "grep hello" → {"grep", "hello", NULL}
char **make_cmd_array(char *command_str)
{
    //I should use split here.

}

// Find PATH=... from envp
char *find_path_in_envp(char **envp)
{
    // example: envp = {"SHELL=/bin/bash", "PATH=/usr/local/bin:/usr/bin:/bin", NULL}
    //output : "/usr/local/bin:/usr/bin:/bin"
    //grep could be: 
    - /usr/local/bin/grep
    - /usr/bin/grep
    - /bin/grep

}

 // Searches for the actual binary executable file for cmd inside all directories from the PATH string.
char *find_command_in_path(char *cmd, char *path)
{
    //esim. input
    - cmd = "grep"
    - path = "/usr/local/bin:/usr/bin:/bin"

    //esim. putput 
    - "/bin/grep"
    //need to find this actualpath
}

// Parses & execs single command: here eg it runs grep
void exe_cmd1(input, cmd, output)
{
    //inside this one, it execute everyhing about
    //analize the av and get actually commands
    char **argv = make_cmd_array(cmd); // {"grep", "hello", NULL}
    //find paht
    char *path = find_path_in_envp(envp);
    //found actually path
    char *full_cmd = find_command_in_path(argv[0], path);
    //execuations
    execve(full_cmd, argv, envp);
    return: 
}
