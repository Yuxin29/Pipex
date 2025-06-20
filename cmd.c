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

//ft_split
//char	**ft_split(char const *s, char c)

char **make_cmd_array(char *command_str)
{
    char    **line;
    char    space;

    space = ' ';
    line = ft_split(char const command_str, space)
    if (!line)
        return (NULL);
    return (line);
}

 // Searches for the actual binary executable file for cmd inside all directories from the PATH string.
char *find_command_in_path(char *cmd, char *path)
{
    //esim. input
    - cmd = "grep"
    - path = "/usr/local/bin:/usr/bin:/bin"

    //esim. output 
    - "/bin/grep"
    //it searches through each path directory to see where the actual binary (like /usr/bin/grep) exists.
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
