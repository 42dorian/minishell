# Changes

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
- [x] need to handle expansions in heredoc only if there are 0 quotes
- [x] properly handle $EMPTY_VAR
- [x] use ft_putstr_fd for env instead of printf
- [x] test heredoc with quotes and everything
- [x] finish built-ins
- [x] handle $?
- [x] handle ""
- [x] handle PATH="" this will try to run commands from current directory 
- [x] handle env -i 
- [x] "echo hi >./outfiles/outfile01 | echo bye" < handle properly
- [x] | echo -n oui < handle this properly
- [x] awk 'BEGIN{for(i=1;i<=10;i++){for(j=1;j<=10;j++){printf("%4d ",i*j)} printf("\n")}}' /dev/null | tail -n 10      < handle this properly
- [ ] update SHLVL
- [ ] valgrind/funcheck
- [ ] run testers
- [ ] cleanup the directories and check for leftover files
- [x] when clicking CTRL+C in "interactive" mode, the exit code updates
- [x] don't use printf to print errors or in general, we need to use ft_putstr_fd so we put errors in fd=2 for errors and fd=1 for normal standard output
- [x] using ARG=1 shouldnt add to the ENV
- [x] handle when path is NULL or empty run executable from curr directory
- [x] on CTRL+C cancel all  heredocs
- [ ] expansion removes singe quotes
- [x] fd leaks
- [ ] need to handle SHLVL
- [x] need to handle env -i, if ran by this just run getcwd and add SHLVL=1
- [x] on error stop at first error
- [x] handle heredoc separately and create expansions there

---

things to do after everything is fixed

- [ ] finish readme
- [ ] remove .supp valgrind file
- [ ] norminette
- [ ] unused function check
- [ ] unused headers check
- [ ] delete this file "Problems.md"
