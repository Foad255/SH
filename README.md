# Simple C Shell

A minimal Unix-like shell written in C. It reads commands from standard input, parses them into arguments, and executes them using `fork()` and `execvp()`.

## Features
- Execute external programs
- Built-in `cd` command
- Built-in `quit` command to exit the shell
- Dynamic prompt showing the current directory
- Displays `~` when inside the home directory

## How It Works
1. The shell prints a prompt.
2. Reads user input using `fgets`.
3. Parses the command into arguments.
4. If the command is built-in (`cd`, `quit`), it executes it directly.
5. Otherwise it creates a child process with `fork()` and runs the command using `execvp()`.
6. The parent process waits for the child to finish using `waitpid()`.

## Compile and Run
```bash
gcc shell.c -o myshell
./myshell
```
## Example
```bash
~/ % ls
~/ % cd /usr
/usr % quit
Good bye from shell
```

