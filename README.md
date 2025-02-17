# smallsh
## Overview
Welcome to the smallsh project! This repository contains the implementation of a simple shell written in C. The smallsh shell offers a subset of the features found in popular shells like bash, allowing you to run commands, handle input/output redirection, and manage processes in both the foreground and background.

## Features
Command Prompt: Provides a prompt for users to run commands just like a conventional shell.
Blank Lines and Comments Handling: Handles blank lines and ignores lines that begin with the # character as comments.
Variable Expansion: Supports variable expansion for $$, which is replaced by the process ID of the shell.

**Built-in Commands:**
* `exit`: Exits the shell.
* `cd`: Changes the current directory.
* `status`: Returns the exit status of the last foreground process.

**Command Execution:**  
Executes other commands by creating new processes using the exec family of functions.  
**Input/Output Redirection:**  
Supports redirection of input and output for commands.  
**Process Management:**  
Foreground and Background Processes: Runs commands in either foreground or background.  
**Signal Handling:**  
Implements custom handlers for the SIGINT and SIGTSTP signals.

## Getting Started
### Prerequisites
A C compiler (like gcc).

### Building the Shell
Clone this repository:
`git clone https://github.com/weemsj/small_shell.git`

### Navigate into the project directory:
cd smallsh

### Compile the program:
`gcc -o smallsh smallsh.c`

### Running the Shell
To start the shell, run the following command:
`./smallsh`

## Usage
Use the shell prompt to enter commands. You can run built-in commands or any other executable commands available in your system path.
Use & to run processes in the background.
Use >, < for output and input redirection respectively.
Press Ctrl+C to send SIGINT or Ctrl+Z to send SIGTSTP to foreground processes.

## Contributions
Contributions are welcome! Feel free to fork this repository, make changes, and submit pull requests for improvements or additional features.

## License
This project is licensed under the MIT License - see the LICENSE file for details.

⸻
Enjoy using smallsh and feel free to provide feedback or ask questions in the issues section of the repository!
