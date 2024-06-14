# Minishell Overview

![](https://raw.githubusercontent.com/R4meau/minishell/master/minishell-quick-demo.gif?token=ADzLiR-sTesle5g6_4CQnHz4RFe69TgDks5ZK6oGwA%3D%3D)

# Introduction

Minishell is a project from the Common Core curriculum of the **42 Network**, designed to deepen us as students' understanding of Unix system programming, shell scripting, and process management. This project involves creating a simple shell that mimics the basic behavior of popular Unix shells **bash**. This project was completed in collaboration with my teammate, <a href="https://github.com/Badered10">Badereddine Aouragh</a>, as it is a pair project.

# Project Overview

Here's an overview of what the Minishell project typically involves:

## Objectives

The primary objectives of the Minishell project are:

* **Understanding Shell Basics**: Gain insights into how a shell operates, including parsing input, executing commands, and handling built-in functionalities.
* **Process Management**: Learn how to manage processes in Unix, including creating, pausing, resuming, and terminating processes.
* **Signal Handling**: Implement signal handling to manage user inputs like Ctrl+C, Ctrl+D, and Ctrl+Z.
* **Error Handling**: Develop robust error handling for invalid inputs and system errors.
* **System Calls**: Become proficient in using system calls related to file manipulation, process control, and inter-process communication.

## Features

Minishell is expected to support the following features:

* **Prompt Display**: Display a prompt to the user for command input.
* **Command Execution**: Execute basic Unix commands, including those with arguments and options.
* **Built-in Commands**: Implement several built-in commands such as cd, echo, pwd, export, unset, env, and exit.
* **Redirections**: Handle input and output redirections (<, <<, >, >>).
* **Pipes**: Support command pipelines using the | operator.
* **Environment Variables**: Manage environment variables and their usage within commands.
* **Signal Handling**: Properly handle signals like SIGINT (Ctrl+C) and EOF (Ctrl+D).
* **Error Messages**: Display appropriate error messages for invalid commands or incorrect usage.

## Project Requirements

To successfully complete the Minishell project, the following requirements must be met:

* **No External Libraries**: The use of external libraries is prohibited. Only standard C libraries are allowed.
* **Memory Management**: Proper management of memory to prevent leaks and ensure efficient use of resources.
* **Modular Code**: Code should be modular, readable, and maintainable, adhering to good coding practices.
* **Compliance**: Follow the project specifications and ensure compliance with the coding standards of the 42 Network.

# Getting Started

## Prerequisites

Before starting the Minishell project, ensure you have a solid understanding of the following concepts:

* Basic Unix commands and shell scripting.
* Process creation and management in Unix (fork, exec, wait).
* File descriptors and redirections.
* Signal handling in Unix.
* C programming, including pointers, memory allocation, and string manipulation.

## Compilation and Execution

To compile and run our Minishell, use the following commands:

```bash
git clone https://github.com/lassachraf/42_minishell.git
```

```bash
cd 42_minishell/
```

```bash
make
```

```bash
./minishell
```

This will build your project and start the shell. You can then enter commands and see the shell's behavior in real-time.

# Conclusion

The Minishell project is an excellent opportunity to enhance your understanding of Unix systems, shell operations, and process management. It is a challenging yet rewarding endeavor that lays a strong foundation for more advanced system programming tasks.

For more details and specific requirements, refer to the project documentation and subject provided by the <a href="https://projects.intra.42.fr/projects/42cursus-minishell">42 Network</a>.

# Project score: (in chae lah)

![](https://blog.kakaocdn.net/dn/ccOHbk/btrmXuGtLet/JzUaL05K8W5AgwZqFkKjC0/img.png)
