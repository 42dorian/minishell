/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 19:02:10 by bguhty            #+#    #+#             */
/*   Updated: 2026/09/17 14:17:33 by bguthy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "clean_ups.c"
#include "copying.c"
#include "count_letters.c"
#include "expansion_helpers_1.c"
#include "expansion_helpers_2.c"
#include "expansion_helpers_3.c"
#include "expansion_helpers_4.c"
#include "env_assign_helpers.c"
#include "handling_expansions.c"
#include "is_quoted.c"
#include "key_value_extraction.c"
#include "letter_checkers_1.c"
#include "letter_checkers_2.c"
#include "letter_checkers_3.c"
#include "preliminary_checkers.c"
#include "quote_removal.c"
#include "rest_helpers.c"
#include "split_helpers.c"
#include "split.c"
#include "stepping_in_input.c"
#include "syntax_error_check.c"
#include "environment_creation.c"
#include "skippers.c"
#include "tokenizing.c"
#include "dollar_sign_handler.c"
#include "special_characters_checkers.c"
#include "word_count_helpers.c"
#include "libft/ft_strlen.c"
#include "libft/list_general.c"
#include "libft/list_helpers.c"
#include "libft/ft_calloc.c"
#include "libft/ft_memset.c"
#include "libft/ft_bzero.c"
#include "libft/ft_itoa.c"
#include "libft/ft_strdup.c"
#include "libft/ft_memcpy.c"

volatile sig_atomic_t	g_signal = 0;

int	determine_quote_type(char letter, int quote_type)
{
    if (quote_type == SINGLE_QUOTE && letter == SINGLE_QUOTE)
        quote_type = 0;
    else if (quote_type == DOUBLE_QUOTE && letter == DOUBLE_QUOTE)
        quote_type = 0;
    else if (quote_type == 0 && letter == SINGLE_QUOTE)
        quote_type = SINGLE_QUOTE;
    else if (quote_type == 0 && letter == DOUBLE_QUOTE)
        quote_type = DOUBLE_QUOTE;
    return (quote_type);
}

int    set_quote_type(int *quote_type, const char letter)
{
    if (is_single_quote(letter) && *quote_type == 0)
    {
        *quote_type = SINGLE_QUOTE;
        return (1);
    }
    if (is_double_quote(letter) && *quote_type == 0)
    {
        *quote_type = DOUBLE_QUOTE;
        return (1);
    }
    if (is_single_quote(letter) && *quote_type == SINGLE_QUOTE)
    {
        *quote_type = 0;
        return (1);
    }
    if (is_double_quote(letter) && *quote_type == DOUBLE_QUOTE)
    {
        *quote_type = 0;
        return (1);
    }
    return (0);
}

char        *get_full_expandable_word_linked_list(const char *line, t_envs *env_list, int len, int *status)
{
    char	*fully_expanded;
	char	*valid_expandable;
	int		i;
	int		quote_flag;

	set_quote_flag_and_index_to_zero(&quote_flag, &i);
	fully_expanded = ft_calloc(sizeof(char), (len + 1));
	if (!fully_expanded)
		return (NULL);
	while (line[i])
	{
		set_quote_flag(&quote_flag, line[i]);
		if (eligible_for_expansion(line, i, quote_flag))
		{
			valid_expandable = get_valid_expandable(line + i + 1);
			make_expansion(fully_expanded, valid_expandable, env_list, status);
			i += (ft_strlen(valid_expandable) + 1);
			free(valid_expandable);
		}
		else
			cat_to_fully_expanded(fully_expanded, line[i++]);
	}
	fully_expanded[len] = 0;
	return (fully_expanded);
}

int    insert_into_linked_list(t_new_token *tokens, const char **split_line)
{
    int words;
    int i;
    t_new_token *next_one;
    t_new_token *new_node;

    i = 0;
    next_one = tokens->next;
    words = len_of_double_pointer(split_line);
    while (i < words)
    {
        new_node = get_new_node(split_line[i], tokens->env_list);
        if (!new_node)
            return (0);
        if (i == 0)
        {
            free(tokens->value);
            tokens->value = normal_copy(split_line[i]);
            if (!tokens->value)
                return (0);
        }
        else
        {
            tokens->next = new_node;
            tokens = new_node;
        }
        i++;
    }
    tokens->next = next_one;
}

int     handle_expansions_linked_list(t_new_token *tokens, int *status)
{
    int         i;
    int         len;
    char        *expanded_line;
    char        **split_again;
    
    while (tokens->next)
    {
        if (!dollar_in_word(tokens->value))
        {
            tokens = tokens->next;
            continue ;
        }
        len = get_full_len_of_expandable(tokens->value, tokens->env_list, status);
        if (len == -1)
            return (0);
        expanded_line = get_full_expandable_word_linked_list(tokens->value, tokens->env_list, len, status);
        if (!expanded_line)
            return (0);
        split_again = split_read_line(expanded_line);
        if (!split_again)
            return (0);
        if (!insert_into_linked_list(tokens, (const char *)split_again))
            return (0);
        tokens = tokens->next;
    }
    return (1);
}

t_new_token *new_minishell(const char *read_line, t_envs *env_list, int *status)
{
    int         i;
    char        **split_line;
    t_new_token *tokens;

    i = 0;
    tokens = NULL;//ft_calloc(1, sizeof(t_new_token));
  //  ft_bzero(tokens, sizeof(t_new_token));
    if (empty_string_and_unclosed_quote_check(read_line, status))
        return (NULL);
    split_line =  split_read_line(read_line);
    if (!split_line)
        return (NULL);
    if (!create_linked_token_struct(&tokens, (const char *)split_line, env_list))
        return (split_clean_up(split_line, word_counter(read_line)), NULL);
    if (!handle_expansions_linked_list(tokens, status))
        return (NULL);
    if (remove_quotes_linked_list(tokens));
    syntax_check_linked_list(tokens, status);
    return (tokens);
}

t_token     *minishell(const char *read_line, t_envs *env_list, int *status)
{
    int     i;
    t_token *tokens;
    char    **split_line;
    char    *expanded_line;

    i = 0;
    if (empty_string_and_unclosed_quote_check(read_line, status))
        return (NULL);
    expanded_line = handle_expansions(read_line, env_list, status);
    if (!expanded_line)
        return (NULL);
    split_line = split_read_line(expanded_line);
    if (!split_line)
        return (free(expanded_line), NULL);    
    tokens = malloc(sizeof(t_token) * (word_counter(expanded_line) + 1));
    if (!tokens)
        return (split_clean_up(split_line, word_counter(expanded_line)), free(expanded_line), NULL);
    if (!create_token_struct(tokens, split_line))
        return (clean_up_tokens_and_split_line(tokens, split_line), free(expanded_line), NULL);
    split_clean_up(split_line, word_counter(expanded_line));
    free(expanded_line);
    if (!remove_quotes(tokens))
        return (free_tokens(tokens), NULL);
    syntax_check(tokens, status);
    if (*status == 2)
        return (free_tokens(tokens), NULL);
    return (tokens);
}

void free_tokens(t_token *token)
{
	int i;

	i = -1;
	if (!token)
		return ;
	while (token[++i].value)
		free((void*)token[i].value);
	free(token);
}

void free_cmds(t_cmds **cmd)
{
	t_cmds *tmp;
	t_cmds *next_cmd;
	if (!cmd || !*cmd)
		return ;
	tmp = *cmd;
	while (tmp)
	{
		next_cmd = tmp->next;
		//free_split(tmp->cmd);
		tmp->cmd = NULL;
		if (tmp->fd_in > 0)
			close(tmp->fd_in);
		if (tmp->fd_out > 1)
			close(tmp->fd_out);
		free(tmp);
		tmp = next_cmd;
	}
	*cmd = NULL;
}

t_new_token *init_token_struct()
{
    t_new_token *new_node;

    new_node = malloc(sizeof(t_new_token *));
    if (!new_node)
        return (NULL);
    ft_bzero(new_node, sizeof(t_new_token));
    return (new_node);
}

int main()//int ac, char **av, const char **envp)
{
	char *line;
	t_shell shell;
	t_new_token *tokens;

    // (void)av;

    // if(ac != 1)
    // {
    // 	ft_putstr_fd("minishell doesn't take arguments\n", STDERR_FILENO);
    //  	return (1);
    // }
    line = "okcso 'here' 'okcso'";
    const char *envp[] = {"BROWSER=/home/guthybarnakoppany/.vscode-server/cli/servers/Stable-618725e67565b290ba4da6fe2d29f8fa1d4e3622/server/bin/helpers/browser.sh",
    "PATH=/home/linuxbrew/.linuxbrew/bin:/home/linuxbrew/.linuxbrew/sbin:/home/bguhty/.local/funcheck/host:/home/bguhty/.vscode-server/extensions/vadimcn.vscode-lldb-1.12.2/bin:/home/bguhty/.vscode-server/bin/645f29cc3176500b4b5762ba887cf2a7f0ffdf2c/bin/remote-cli:/home/linuxbrew/.linuxbrew/bin:/home/linuxbrew/.linuxbrew/sbin:/home/bguhty/.local/funcheck/host:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/usr/lib/wsl/lib:/mnt/c/Python314/Scripts/:/mnt/c/Python314/:/mnt/c/Program Files (x86)/NVIDIA Corporation/PhysX/Common:/mnt/c/Program Files (x86)/Razer Chroma SDK/bin:/mnt/c/Program Files/Razer Chroma SDK/bin:/mnt/c/Program Files (x86)/Common Files/Oracle/Java/javapath:/mnt/c/Windows/system32:/mnt/c/Windows:/mnt/c/Windows/System32/Wbem:/mnt/c/Windows/System32/WindowsPowerShell/v1.0/:/mnt/c/Windows/system32/config/systemprofile/AppData/Local/Microsoft/WindowsApps:/mnt/c/WINDOWS/system32:/mnt/c/WINDOWS:/mnt/c/WINDOWS/System32/Wbem:/mnt/c/WINDOWS/System32/WindowsPowerShell/v1.0/:/mnt/c/WINDOWS/System32/OpenSSH/:/mnt/c/Program Files/dotnet/:/mnt/c/Program Files/nodejs/:/mnt/c/ProgramData/chocolatey/bin:/mnt/c/Program Files/Git/cmd:/mnt/c/Users/Computer/AppData/Local/Programs/Python/Python311/Scripts/:/mnt/c/Users/Computer/AppData/Local/Programs/Python/Python311/:/mnt/c/Users/Computer/AppData/Local/Microsoft/WindowsApps:/mnt/c/Users/Computer/AppData/Local/Programs/Microsoft VS Code/bin:/mnt/c/Users/Computer/AppData/Roaming/npm:/snap/bin",
    "DBUS_SESSION_BUS_ADDRESS=unix:path=/run/user/501/bus",
    "TERM_PROGRAM=vscode", NULL};
	ft_bzero(&shell, sizeof(shell));
	if (!add_envp_to_list(&shell.env_list, envp))
        return (1);
	// init_interactive_signals();
	// while ((line = readline("minishell$ ")))
	// {
	// 	if (line[0] != '\0' || !line)
	// 		add_history(line);
	// 	if (WTERMSIG(g_signal) != 0)
	// 	{
	// 		shell.status = 128 + WTERMSIG(g_signal);
	// 		g_signal = 0;
	// 	}
	// 	tokens = minishell(line, shell.env_list, &shell.status);
	// 	free(line);
	// 	if (!tokens)
	// 		continue;
	// 	shell.cmds = build_cmds(tokens, shell.env_list, &shell);
	// 	free_tokens(tokens);
	// 	if (!shell.cmds)
	// 		continue;
	// 	shell.status = execute_cmds(&shell);
	// 	free_cmds(&shell.cmds);
	// }
	// rl_clear_history();
    tokens = new_minishell(line, shell.env_list, &shell.status);
    // if (tokens)
    // {
    //     clean_up_tokens_and_split_line(tokens, NULL);
    //     clean_up_env_list(&shell.env_list);
    // }
	// ft_putstr_fd("exit\n", STDERR_FILENO);
	// close(0);
	// close(1);
	// close(2);
    // free_all_and_exit(&shell, shell.status);
}
