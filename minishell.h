/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguhty <bguhty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 13:20:37 by bguhty            #+#    #+#             */
/*   Updated: 2026/09/09 15:57:11 by bguhty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define a 97
# define A 65
# define z 122
# define Z 90
# define SPACE 32
# define HORIZONTAL_TAB 9
# define CARRIAGE_RET 13
# define SINGLE_QUOTE 39
# define DOUBLE_QUOTE 34
# define EQUAL_SIGN 61
# define DOLLAR_SIGN 36
# define REDIR_IN 60
# define REDIR_OUT 62
# define PIPE 124
# define QUESTION_MARK 63
# define PATH_MAX 1024

# include "./libft/libft.h"
# include <errno.h>
# include <limits.h>
# include <stdio.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <fcntl.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <unistd.h>
# include <signal.h>

extern volatile sig_atomic_t g_signal;
typedef struct s_cmds	t_cmds;
typedef struct s_envs	t_envs;

typedef struct s_shell
{
	t_cmds *cmds;
	t_envs *env_list;
	char **envp;
	int status;
}t_shell;

typedef struct s_cd
{
	char	*target_path;
	char	old_path[PATH_MAX];
	char	new_path[PATH_MAX];
}			t_cd;

typedef enum e_token_type
{
	token_word,
	token_pipe,
	token_redirect_in,
	token_redirect_out,
	token_append,
	token_heredoc,
	token_env_assign
}						t_token_type;

typedef struct s_cmds
{
	char				**cmd;
	int 				fd_in;
	int					fd_out;
	int					exit_status;
	t_cmds				*next;
	pid_t				pid;
	char				**envp;
}						t_cmds;

typedef struct s_token
{
    const char		*value;
	int				quoted;		
    t_token_type    type;
}                   t_token;


void					*ft_calloc(size_t nmemb, size_t size);
int						string_compare(const char *string1,
							const char *string2);
int						is_number(char letter);
int						is_upper_case(char letter);
int						is_lower_case(char letter);
int						fill_up_double_pointer(char **split_line,
							const char *read_line);
char					**split_read_line(const char *read_line);
char					**allocating_double_pointer(const char *read_line);
int						word_counter(const char *read_line);
int    					add_envp_to_list(t_envs **my_list, const char **envp);
int						is_word_2(const char *read_line, int *i, int *words);
int     				clean_up_token_and_env_list(t_token *tokens, t_envs **env_list);
int						check_for_quote(const char letter, int *quote_type);
void					skip_white_spaces(const char *read_line, int *i);
int						is_white_space(const char letter);
void					skip_to_next_quote(const char *read_line, int *i,
							char quote_type);
void					skip_non_white_spaces(const char *read_line, int *i);
void					split_clean_up(char **split_line, int i);
int						env_assign_check(char *string);
void					get_real_quote_type(const char *word, int *quote_type,
							int *i);
char    				*normal_copy(const char *get_copied);
t_envs					*env_list_addition(t_token *tokens, t_envs *env_list);
int						dollar_sign_exception(const char *read_line, int *i,
							int *words);
int						is_heredoc_or_append(const char letter1,
							const char letter2);
int						count_letters_on_special_character(const char *read_line, int i,
							int *letters);
int						is_special_character(const char letter1, const char letter2);
int						is_redir_or_pipe(const char letter);
int						quote_in_word(const char *read_line, int *i,
							int *words);
int						syntax_error_message_display(const char *token_value);
int						is_dollar_sign(const char letter);
int						is_pipe(const char letter);
int     				is_single_quote(const char letter);
int						is_double_quote(const char letter);
void					syntax_check(t_token *tokens, int *status);
int						tokenizer(char *input);
int						create_token_struct(t_token *tokens, char **line);
int						check_for_redirect_out_and_append(const char *read_line,
							int *i);
int						check_for_redirect_in_and_heredoc(const char *read_line,
							int *i);
int						check_for_pipe(const char *read_line, int *i);
int						dollar_is_standing_alone(const char letter);
void					process_after_dollar_sign(const char *read_line, int *i,
							int *words);
int						is_dollar_after_dollar(const char letter);
int						is_redir(const char letter);
int						is_redir_in(const char letter);
int						is_redir_out(const char letter);
int						handle_expansions(t_envs *env_list, t_token *tokens, int *exit_code);
int						is_astrisk(const char letter);
int						is_terminator(const char letter);
int						letter_after_dollar_is_num_or_astrisk(const char letter);
int						is_underline(char letter);
int						is_white_space_or_special_character(const char letter);
int						key_counter(const char *envp);
char    				*get_full_expandable_word(t_token curr_token, t_envs *env_list, int len, int *exit_code);
int     				get_full_len_of_expandable(t_token curr_token, t_envs *env_list, t_token *tokens, int *exit_code);
int     				get_len_of_current_expandable(const char *expandable, t_envs *env_list, int *exit_code);
int						digit_counter(pid_t pid);
char					*find_cmd_path(char *cmd_name, char **split_path,
							int *status);
void					free_split(char **strs);
char					*handling_path(char *cmd_name, char *path, int *exit_status);

int						check_access(char *cmd);
int						check_cmd(char *cmd);
void	print_error(char *msg, char *cmd, char *arg, int fd);
int						is_dir(char *cmd, struct stat *path_stat);
int						pwd(void);
void free_cmd(t_cmds *cmds);
t_cmds	*new_cmd(void);
t_cmds	*add_cmd(t_cmds *head, t_cmds *new_list);
int		add_arg_to_cmd(t_cmds *node, const char *arg);

int	process_token(t_cmds **head, t_cmds **cur, t_token *t, int *i, t_envs *env);

int		handle_in(t_cmds *curr, t_token *tokens, int *i);
int		handle_out(t_cmds *curr, t_token *tokens, int *i);
int		handle_pipe(t_cmds **head, t_cmds **curr);

int	handle_heredoc(t_cmds *curr, t_token *token, int *i, t_envs *env);

t_cmds	*build_cmds(t_token *tokens, t_envs *env);
int find_path(char **envp);

int	execute_cmds(t_shell *shell);
int run_cmd(t_cmds *cmd, char **envp, int *status);
int is_built_in(char *cmd);
int	restore_io(int saved_stdin, int saved_stdout);

int	execute_single_cmd(t_shell *shell);
int change_io(t_cmds *cmds);
void	child_redirections(t_cmds *cmds, int *fd, int stored_input);
void close_inherited_fds(t_cmds *cmds);
void safe_dup2(int oldfd, int newfd);
// void run_child(t_cmds *cmds, int *fd, int stored_input, char **envp);
void	run_child(t_cmds *cmds, int *fd, int stored_input, t_shell *shell);

void clean_parent(t_cmds *cmds, int *fd, int *stored_input);
void wait_pids(t_cmds *cmds, int *status);
// int	run_built_in(t_cmds *cmd, t_envs *env_list);
int	run_built_in(t_cmds *cmd, t_envs *env_list, t_shell *shell);
int echo(t_cmds *cmd);
void wait_single_pid(pid_t pid, int *status, int last_pid);

int     is_question_mark(const char letter);
int     value_counter(const char *envp);
int     is_quote(const char letter);

t_envs     *copy_from_envp_to_own_env_list(const char **envp, int i);

//added these
int     is_valid_after_dollar_sign(const char letter);
void    copy_till_next_quote(const char *read_line, int *i, char *new_word, int *new_index);
int     is_end(const char letter);
void init_interactive_signals(void);
void init_execution_signals(void);
void pause_interactive_signals(void);
void init_heredoc_signals(void);
char **create_envp(t_envs *env_list);
int env(t_envs *env_list);
int export_bi(t_cmds *cmd, t_envs **env_list);
int unset(t_cmds *cmd, t_envs **env_list);
int exit_bi(t_cmds *cmd, t_shell *shell);
int valid_identifier(char *key);
int cd_bi(t_cmds *cmd, t_envs **env_list);
int		update_or_add(t_envs **env_list, char *value, char *key);
#endif
