# Pipex

**Pipex** is a C project from the 42 curriculum that simulates shell pipe behavior. It executes multiple commands in sequence, connecting them via pipes, and handles input/output redirection between files, replicating shell functionality like `< infile cmd1 | cmd2 | ... | cmdn > outfile`. The project reinforces process creation, `fork()`, `execve()`, and pipe management.

## Features

- Executes multiple commands connected with pipes  
- Handles input and output file redirection  
- Graceful error handling for command not found or invalid files  
- Focuses on C process management and inter-process communication  

## Build & Run

1. Clone the Module Directory

```
git clone https://github.com/Yuxin29/pipex.git
cd pipex
```

2.  Compile using the Makefile:

```
make
# Generates the executable pipex
```

3. Run the program:

```
./pipex infile "cmd1" "cmd2" ... "cmdn" outfile
 Example:
./pipex input.txt "grep hello" "wc -l" output.txt
```

## Project Structure

```
include/
├── pipex.h
src/
├── cmd.c
├── helper.c
├── pipe.c
Makefile
README.md

```
