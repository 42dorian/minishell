*This project has been created as part of the 42 curriculum by dabdulla, bguthy.*

# Minishell

## Description
Minishell is a comprehensive 42 school project tasked with creating a functional mini shell. The objective is to dive deep into systems programming, focusing heavily on process creation, file descriptor routing, signal handling, and environment variable manipulation.
This project does not aim to be fully Bash-compatible. Behavior that is not explicitly listed below should be considered out of scope.

## Features
- **Prompt:** Displays a working prompt while waiting for a new command.
- **History:** Maintains command history using the `readline` library.
- **Execution:** Locates and executes the correct binary based on the `PATH` variable or via relative/absolute paths.
- **Quoting:** Handles single quotes (`'`) and double quotes (`"`) correctly to prevent or allow environment variable expansion.
- **Redirections:**
  - `<` redirects input.
  - `>` redirects output.
  - `<<` creates a heredoc (reading input until a delimiter is seen).
  - `>>` redirects output in append mode.
- **Pipes:** Implements piping (`|`), routing the output of one command directly into the input of the next.
- **Expansions:** Handles environment variables (e.g., `$USER`) and `$?` for the exit status of the most recently executed foreground pipeline.
- **Signals:** Handles `ctrl-C`, `ctrl-D`, and `ctrl-\` correctly, mimicking default bash behavior.

## Built-in Commands
The shell natively supports the following built-in commands without relying on external system binaries:
* `echo` (with the `-n` option)
* `cd` (with a relative or absolute path)
* `pwd` (no options)
* `export` (no options)
* `unset` (no options)
* `env` (no options or arguments)
* `exit` (no options)

## Instructions

### Compilation
Clone the repository and compile the project by running `make` at the root directory:

    make

### Execution
Run the compiled binary to enter the shell:

    ./minishell

## Resources

* https://www.youtube.com/watch?v=kDxjcyHu_Qs
* https://www.youtube.com/watch?v=ss1-REMJ9GA
* https://www.youtube.com/watch?v=6xbLgZpOBi8
* https://www.youtube.com/watch?v=NkfIUo_Qq4c
* https://www.youtube.com/watch?v=5fnVr-zH-SE
* https://www.gnu.org/savannah-checkouts/gnu/bash/manual/bash.html

## AI Usage

During the development of this project, Artificial Intelligence tools were utilized strictly in accordance with 42 school guidelines. AI was used as a learning assistant for:
* Generating shell edge-case scenarios to build a robust testing suite.
* Artificial Intelligence was used to assist in creating this README file.
