# Pipex

Description

pipex is a simplified shell pipeline emulator written in C, designed to replicate basic Unix pipe functionality.

The program executes two commands with an input and output file, connecting the first command's output to the second command's input via a pipe, similar to: < infile cmd1 | cmd2 > outfile

Exit Codes
    0: Success
    1: General error (e.g., file open failure)
    126: Command found but not executable (permission denied)
    127: Command not found
    128 + signal: If child terminated by signal (e.g., segmentation fault)

Known Limitations

    Supports exactly two commands in the pipeline
    Does not support shell built-in commands (e.g., cd, export)
    No support for shell expansions or quotes parsing
    Minimal error messages without colors or formatting
    False positive in some valrind test from system leak
    

---------------------new variable types used here--------------------------------
- pid_t is a data type used in POSIX systems (like Linux and macOS) to represent process IDs. 
    - pid_t is a typedef (an alias) for some integer type, usually a signed integer like int.
    - It is the type returned by process-related system calls such as fork(), getpid(), and waitpid().

---------------------allower external functions-----------------------
open, close, read, write, malloc, free, perror, strerror, access, dup, dup2,
execve, exit, fork, pipe, unlink, wait, waitpid

--------------below are what are new to me---------------
- xdg-open: opens a file or URL in the user's preferred application
    xdg-open {file | URL}
- close: close a file descriptor
    int close(int fd);
- read: read — read from standard input into shell variables
- perror - print a system error message
   void perror(const char *s);
- strerror: return string describing error number
    char *strerror(int errnum);
- access: check user's permissions for a file
    int access(const char *pathname, int mode);
- dup, dup2: duplicate a file descriptor
    int dup(int oldfd);
    int dup2(int oldfd, int newfd);
- execve - execute program
    int execve(const char *pathname, char *const argv[], char *const envp[]);
- exit: cause the shell to exit
- fork
    pid_t fork(void);
    On success, the PID of the child process is returned in the parent, and 0 is returned  in  the  child.
       On failure, -1 is returned in the parent, no child process is created, and errno is set appropriately.
- pipe: Creates a unidirectional communication channel (a pipe) between processes
    usually between a parent and child after a fork().
    - pipefd[0]: the read end of the pipe
    - pipefd[1]: the write end of the pipe
    sysnoiphasise: int  pipe(int pipefd[2]);  
                   fd[2]: an array of 2 ints
                   -return values: 0 on success and -1 on fai
- unlink: call  the  unlink function to remove the specified file
- wait, waitpid, waitid - wait for process to change state
    #include <sys/types.h>
    #include <sys/wait.h>
    pid_t wait(int *wstatus);
    pid_t waitpid(pid_t pid, int *wstatus, int options);
