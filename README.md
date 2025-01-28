# Minishell

This is a fork of the **Minishell** project, originally developed by [Kartik](https://github.com/KartikPat250905) and [Aaron](https://github.com/Deblish) as part of the School 42 curriculum. It implements a basic Unix shell with functionalities such as command execution, built-in commands, environment variable handling, pipes, redirections, and signal handling.

The project aims to provide a deeper understanding of system programming concepts, including process management, memory handling, and the usage of Unix system calls.
## Features

This version of **Minishell** implements the mandatory part of the project subject, and the features include:

1. **Interactive Shell**: 
   - The shell runs in an interactive mode, displaying a prompt and waiting for user commands.

2. **Command Execution**: 
   - Execute binaries in the `$PATH` (e.g., `ls`, `echo`, `cat`) with arguments.

3. **Built-in Commands**:
   - `echo` (with `-n` option)
   - `cd`
   - `pwd`
   - `export`
   - `unset`
   - `env`
   - `exit`

4. **Environment Variables**:
   - Manage and expand environment variables during execution.

5. **Pipes (`|`)**:
   - Support for simple pipelines between commands.

6. **Redirections**:
   - Input redirection (`<`), output redirection (`>`), and append redirection (`>>`).

7. **Signal Handling**:
   - Handle `Ctrl+C`, `Ctrl+D`, and `Ctrl+\` gracefully without quitting the shell.

8. **Error Handling**:
   - Display meaningful error messages for invalid commands, permission issues, and syntax errors.

## Getting Started

### Prerequisites
To compile and run **Minishell**, ensure you have:
- A Unix-based operating system (Linux or macOS)
- `gcc` or another C compiler
- `make`

### Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/KartikPat250905/minishell.git
   cd minishell
