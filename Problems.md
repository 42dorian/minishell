# Changes

Added this to *minishell* function:
```
if (*read_line == '\0')
	return (NULL);
```

This fixes a segfault, also it makes it possible when pressing enter, it just continues in new line.

added this to string_compare:
```
if (string1[i] == '\0' && string2[i] == '\0')
	return (1);
```
this fixed an issue where the > would only append >> so i added a check to see if its the correct size as well

delete maini.c

Problems, tests and TO-DOs

- [x] echo with -n option
- [x] cd 
- [x] pwd
- [x] export
- [x] unset 
- [x] env
- [x] exit
- [x] 2: Syntax error should set the exitcode to 2
- [x] 127: Command not found (or no such file or directory) 
- [x] 126: Permission denied (or is a directory) 
- [x] 130: Killed by sigint 
- [x] 131: killed by sigquit
- [ ] need to handle expansions in heredoc only if there are 0 quotes
- [ ] properly handle $EMPTY_VAR
- [x] use ft_putstr_fd for env instead of printf
- [x] test heredoc with quotes and everything
- [x] finish built-ins
- [x] handle $?
- [ ] valgrind/funcheck
- [ ] run testers
- [ ] cleanup the directoryies and check for leftover files
- [ ] when clicking CTRL+C in "interactive" mode, the exit code updates
- [ ] don't use printf to print errors or in general, we need to use ft_putstr_fd so we put errors in fd=2 for errors and fd=1 for normal standard output
- [x] using ARG=1 shouldnt add to the ENV

---

things to do after everything is fixed

- [ ] finish readme
- [ ] norminette
- [ ] unused function check
- [ ] unused headers check
- [ ] delete this file "Problems.md"
