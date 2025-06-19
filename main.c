/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuwu <yuwu@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 19:47:21 by yuwu              #+#    #+#             */
/*   Updated: 2025/06/19 19:48:20 by yuwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// the relationship of fork and parent/child process

/*
Parent Process (main)
               |
    -------------------------------
    |                             |
fork()                         fork()
    |                             |
Child 1 (exec cmd1)         Child 2 (exec cmd2)
*/

//main process, excution function
- av checker
- check path and environments
- create pipe
- execute cmd1
- execute cmd2
- exit

int	main(int ac, char **av)
{

}
