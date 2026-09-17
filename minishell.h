/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 13:20:37 by bguhty            #+#    #+#             */
/*   Updated: 2026/09/17 22:52:29 by bguthy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define LOWER_CASE_A 97
# define A 65
# define LOWER_CASE_Z 122
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
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <sys/stat.h>
# include <sys/wait.h>

extern volatile sig_atomic_t	g_signal;
typedef struct s_cmds			t_cmds;
typedef struct s_envs			t_envs;

typedef struct s_shell
{
	t_cmds						*cmds;
	t_envs						*env_list;
	char						**envp;
	int							status;
	int							saved_stdin;
	int							saved_stdout;
}								t_shell;

typedef struct s_cd
{
	char						*target_path;
	char						old_path[PATH_MAX];
	char						new_path[PATH_MAX];
}								t_cd;

typedef enum e_token_type
{
	token_word,
	token_pipe,
	token_redirect_in,
	token_redirect_out,
	token_append,
	token_heredoc,
	token_env_assign,
	token_invalid = -1
}								t_token_type;

typedef struct s_cmds
{
	char						**cmd;
	int							fd_in;
	int							fd_out;
	int							exit_status;
	t_cmds						*next;
	t_cmds						*prev;
	pid_t						pid;
}								t_cmds;

typedef struct s_token
{
	char						*value;
	int							quoted;
	int							export_flag;
	t_token_type				type;
}								t_token;

void							*ft_calloc(size_t nmemb, size_t size);
int								string_compare(const char *string1,
									const char *string2);
int								is_number(char letter);
int								is_upper_case(char letter);
int								is_lower_case(char letter);
int								fill_up_double_pointer(char **split_line,
									const char *read_line);
char							**split_read_line(char *read_line);
char							**allocating_double_pointer(const char
									*read_line);
int								add_envp_to_list(t_envs **my_list,
									const char **envp);
int								check_if_and_set_quote(const char letter,
									int *quote_type);
void							skip_white_spaces(const char *read_line,
									int *i);
int								is_white_space(const char letter);
void							split_clean_up(char **split_line);
char							*normal_copy(const char *get_copied);
int								dollar_sign_exception(const char *read_line,
									int *i, int *words);
int								is_heredoc_or_append(const char letter1,
									const char letter2);
int								count_letters_on_special_character(const char
									*read_line, int i, int *letters);
int								is_redir_or_pipe(const char letter);
int								syntax_error_message_display(const char
									*token_value);
int								is_dollar_sign(const char letter);
int								is_pipe(const char letter);
int								is_single_quote(const char letter);
int								is_double_quote(const char letter);
int								syntax_check(t_token *tokens, int *status);
int								tokenizer(char *input);
int								create_token_struct(t_token *tokens,
									char **line);
int								check_for_redirect_out_and_append(
									const char *read_line, int *i);
int								check_for_redirect_in_and_heredoc(
									const char *read_line, int *i);
int								check_for_pipe(const char *read_line, int *i);
int								dollar_is_standing_alone(const char letter);
void							process_after_dollar_sign(const char *read_line,
									int *i, int *words);
int								is_redir(const char letter);
int								is_redir_in(const char letter);
int								is_redir_out(const char letter);
int								is_astrisk(const char letter);
int								is_terminator(const char letter);
int								letter_after_dollar_is_num_or_astrisk(
									const char letter);
int								is_underline(char letter);
int								is_white_space_or_special_character(
									const char letter);
int								key_counter(const char *envp);
int								get_len_of_current_expandable(
									const char *expandable,
									t_envs *env_list, int *exit_code);
char							*find_cmd_path(char *cmd_name,
									char **split_path, int *status);
void							free_split(char **strs);
char							*handling_path(char *cmd_name, char *path,
									int *exit_status);
void							assign_exit_status(int status,
									int *exit_status);
int								check_access(char *cmd);
int								check_cmd(char *cmd);
void							print_error(char *msg, char *cmd, char *arg,
									int fd);
int								is_dir(char *cmd, struct stat *path_stat);
int								pwd(void);
void							free_cmd(t_cmds *cmds);
t_cmds							*new_cmd(void);
int								add_arg_to_cmd(t_cmds *node, const char *arg);

int								process_token(t_cmds **cur, t_token *t, int *i,
									t_envs *env);

int								handle_in(t_cmds *curr, t_token *tokens,
									int *i);
int								handle_out(t_cmds *curr, t_token *tokens,
									int *i);
int								handle_pipe(t_cmds **curr);

int								handle_heredoc(t_cmds *curr, t_token *token,
									int *i, t_envs *env);

t_cmds							*build_cmds(t_token *t, t_envs *env,
									t_shell *shell);
int								find_path(char **envp);

int								execute_cmds(t_shell *shell);
int								run_cmd(t_cmds *cmd, char **envp, int *status,
									t_shell *shell);
void							execute_pipeline(t_shell *shell,
									t_cmds *curr_cmd, int *fd, int *st_in);
int								is_built_in(char *cmd);
void							exec_child_process(t_cmds *cmd, char *path,
									char **envp, t_shell *shell);
int								restore_io(int saved_stdin,
									int saved_stdout);
void							check_child_fds(t_cmds *cmds,
									int *fd, int stored_input,
									t_shell *shell);
int								fork_pipe(t_cmds *cmds, int *fd,
									int *stored_input, t_shell *shell);
int								execute_single_cmd(t_shell *shell);
int								change_io(t_cmds *cmds);
void							child_redirections(t_cmds *cmds, int *fd,
									int stored_input);
void							close_inherited_fds(t_cmds *cmds);
void							safe_dup2(t_cmds *cmd, int oldfd, int newfd);
void							run_child(t_cmds *cmds, int *fd,
									int stored_input, t_shell *shell);

void							clean_parent(t_cmds *cmds, int *fd,
									int *stored_input);
void							wait_pids(t_cmds *cmds, int *status);
int								run_built_in(t_cmds *cmd, t_envs *env_list,
									t_shell *shell);
int								echo(t_cmds *cmd);
void							wait_single_pid(pid_t pid, int *status,
									int last_pid);

int								is_question_mark(const char letter);
int								value_counter(const char *envp);
int								is_quote(const char letter);
t_envs							*copy_from_envp_to_own_env_list(
									const char **envp, int i);
int								is_valid_after_dollar_sign(
									const char letter);
void							copy_till_next_quote(
									const char *read_line,
									int *i, char *new_word, int *new_index);
int								is_end(const char letter);
void							init_interactive_signals(void);
void							init_execution_signals(void);
void							pause_interactive_signals(void);
void							init_heredoc_signals(void);
void							heredoc_sigint(int sig);
void							print_heredoc_warning(const char *eof);
char							*expanded_line(char *line, t_envs *env);
char							**create_envp(t_envs *env_list);
int								env(t_envs *env_list);
int								export_bi(t_cmds *cmd, t_envs **env_list);
int								unset(t_cmds *cmd, t_envs **env_list);
int								exit_bi(t_cmds *cmd, t_shell *shell);
int								valid_identifier(char *key);
int								cd_bi(t_cmds *cmd, t_envs **env_list);
int								update_or_add(t_envs **env_list, char *value,
									char *key);
void							free_tokens(t_token *token);
void							free_cmds(t_cmds **cmd);
void							free_all_and_exit(t_shell *shell, int status);

int								get_full_len_of_expandable(
									const char *read_line,
									t_envs *env_list, int *exit_code);
char							*get_full_expandable_word(const char *read_line,
									t_envs *env_list, int len, int *exit_code);
char							*handle_expansions(t_envs *env_list,
									const char *read_line,
									int *exit_code);
int								dollar_in_word(const char *word);
int								count_valid_characters_after_dollar_sign(
									const char *curr_expandable);
void							set_quote_flag(int *quote_flag,
									const char letter);
char							*get_valid_expandable(const char *expandable);
void							make_expansion(char *fully_expnaded,
									const char *mock_expand, t_envs *env_list,
									int *exit_code);
void							cat_to_fully_expanded(char *fully_expanded,
									const char new_letter);
int								clean_up_env_list(t_envs **env_list);
int								is_delimeter(const char letter);
int								word_counter(const char *read_line);
int								check_for_special_character(
									const char *read_line,
									int *i, int *words);
void							move_index_and_set_curr_len_to_zero(
									const char *read_line,
									int *i, int *curr_len);
void							set_quote_flag_and_index_to_zero(
									int *quote_flag, int *i);
int								double_dollar_or_question_mark_check(
									const char letter);
int								get_len_of_real_env(const char *test_env,
									t_envs *env_list);
int								get_len_of_valid_expandable(
									const char *expandable);
char							*get_value(const char *envp);
char							*get_key(const char *envp);
void							set_curr_and_total_len_to_zero(int *curr_len,
									int *total_len);
int								eligible_for_expansion(const char *read_line,
									int i, int quote_flag);
int								check_res_of_curr_len_and_increment_accordingly(
									int *curr_len,
									int *total_len, int *i,
									const char *read_line);
char							*get_from_my_env_list(
									const char *expandable,
									t_envs env_list);
char							*convert_pid_to_string(void);
void							set_quote_type(int *quote_type,
									const char letter);
char							*copy_till_next_word(const char *read_line,
									int *i);
int								empty_string_and_unclosed_quote_check(
									const char *read_line, int *status);
void							clean_up_tokens_and_split_line(
									t_token *tokens, char **split_line);
int								remove_quotes(t_token *tokens);
int								solo_standing_special_character(
									const char *read_line, int *i);
int								preliminary_check(t_token *tokens);
int								count_chars_without_quotes(
									const char *quoted_word);
int								count_letters_till_next_word(
									const char *read_line, int i);
void							increment_total_len_and_index_by_one(
									int *total_len, int *i);
int								how_many_digits(int *number);
int								create_env(char *key, char *value,
									t_envs **env_list);
int								token_list_size(t_token *tokens);
int								len_of_split_line(char **read_line);
int								loop_for_unclosed_quotes(
									const char *read_line);
int								display_unclosed_quote_error_message(
									int *status);
int								put_shlvl_in_env_list(t_envs **my_list);
char							*extract_value_from_env_list(t_envs **my_list,
									const char *missing_key);
int								in_env_list(t_envs **my_list,
									char *missing_key);

#endif
