/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguhty <bguhty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 19:02:10 by bguhty            #+#    #+#             */
/*   Updated: 2026/09/12 12:49:40 by dabdulla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// #include "env_assign_helpers.c"
// #include "rest_helpers.c"
// #include "split_helpers.c"
// #include "split.c"
// #include "stepping_in_input.c"
// #include "syntax_error_check.c"
// #include "expansion_check.c"
// #include "environment_creation.c"
// #include "skippers.c"
// #include "tokenizing.c"
// #include "dollar_sign_handler.c"
// #include "is_special_character.c"
// #include "special_characters_checkers.c"
// #include "word_count_helpers.c"
// #include "libft/ft_strlen.c"
// #include "libft/list_general.c"
// #include "libft/list_helpers.c"
// #include "libft/ft_calloc.c"
// #include "libft/ft_memset.c"
// #include "libft/ft_bzero.c"
// #include "libft/ft_itoa.c"
// #include "libft/ft_strdup.c"
// #include "libft/ft_memcpy.c"

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

int     count_valid_char(const char *quoted_word)
{
    int i;
    int counter;
    int quote_type;

    quote_type = 0;
    counter = 0;
    i = 0;
    while (quoted_word[i])
    {
        get_real_quote_type(quoted_word, &quote_type, &i);
        if (quoted_word[i] != quote_type)
        {
            counter++;
            i++;
        }
    }
    return (counter);
}

void    get_real_quote_type(const char *word, int *quote_type, int *i)
{
    while (check_for_quote(word[*i], quote_type) && word[*i] == word[(*i) + 1])
        (*i)++;
    if (!check_for_quote(word[*i], quote_type))
        return ;
    if (*quote_type != 0)
    {
        if ((*quote_type = determine_quote_type(word[*i], *quote_type)) == 0)
            (*i)++;
    }
    else
    {
        *quote_type = determine_quote_type(word[*i], *quote_type);
        (*i)++;
    }
}

int    malloc_for_new_word(char **new_word, int len)
{
    *new_word = malloc(sizeof(char) * (len + 1));
    if (!new_word)
        return (0);
    else
        return (1);
}

char    *get_rid_of_them_quotes(t_token *tokens, int i)
{
    int     j;
    int     quote_type;
    int     local_index;
    char    *new_word;

    quote_type = 0;
    j = 0;
    local_index = 0;
    if (!malloc_for_new_word(&new_word, count_valid_char(tokens[i].value)))
        return (free((void*)tokens[i].value), NULL);
    while (tokens[i].value[j])
    {
        if (check_for_quote(tokens[i].value[j], &quote_type))
        {
            j++;
            while (tokens[i].value[j] != quote_type && tokens[i].value[j])
                new_word[local_index++] = tokens[i].value[j++];
            j++;
        }
        else if (tokens[i].value[j])
            new_word[local_index++] = tokens[i].value[j++];
    }
    new_word[local_index] = 0;
    free((void*)tokens[i].value);
    return (new_word);
}

int    remove_quotes(t_token *tokens)
{
    int i;
    int j;

    i = 0;
    j = 0;
    while (tokens[i].value)
    {
        while (tokens[i].value[j])
        {
            if (is_quote(tokens[i].value[j]))
            {
                tokens[i].quoted = 1;
                tokens[i].value = get_rid_of_them_quotes(tokens, i);
                if (!tokens[i].value)
                    return (0);
                break ;
            }
            j++;
        }
        i++;
        j = 0;
    }
    return (1);
}

void    copy_key(const char *envp, char *new_key)
{
    int i;

    i = 0;
    while (envp[i] != EQUAL_SIGN)
    {
        new_key[i] = envp[i];
        i++;
    }
    new_key[i] = 0;
}

char    *insert_key(const char *envp)
{
    char    *new_key;

    new_key = malloc(sizeof(char) * (key_counter(envp) + 1));
    if (!new_key)
        return (NULL);
    copy_key(envp, new_key);
    return (new_key);
}

void    copy_value(const char *envp, char *new_value)
{
    int i;
    int j;

    i = 0;
    j = 0;
    while (envp[i] != EQUAL_SIGN)
        i++;
    i++;
    while (envp[i])
        new_value[j++] = envp[i++];
    new_value[j] = 0;
}

char    *insert_value(const char *envp)
{
    char    *new_value;

    new_value = malloc(sizeof(char) * (value_counter(envp) + 1));
    if (!new_value)
        return (NULL);
    copy_value(envp, new_value);
    return (new_value);
}

int     clean_up_env_list(t_envs **env_list)
{
    ft_lstclear(env_list, free);
    return (0);
}

int     get_env_len(const char **envp)
{
    int len;

    len = 0;
    while (envp[len])
        len++;
    return (len);
}

int    add_envp_to_list(t_envs **my_list, const char **envp)
{
    t_envs *new_node;
    int     i;
    int     envp_len;

    i = 0;
    envp_len = get_env_len(envp);
    while (i < envp_len)
    {
        new_node = copy_from_envp_to_own_env_list(envp, i);
        if (!new_node)
            return(clean_up_env_list(my_list));
        else
            ft_lstadd_back(my_list, new_node);
        i++;
    }
    return (1);
}

t_envs     *copy_from_envp_to_own_env_list(const char **envp, int i)
{
    t_envs *new_node;

    new_node = malloc(sizeof(t_envs));
    if (!new_node)
        return (NULL);
    new_node->key = insert_key(envp[i]);
    if (!new_node->key)
        return (free(new_node), NULL);
    new_node->value = insert_value(envp[i]);
    if (!new_node->value)
        return (free(new_node->key), free(new_node), NULL);
    new_node->next = NULL;
    return (new_node);
}

void        clean_up_tokens_and_split_line(t_token *tokens, char **split_line)
{
    int i;

    i = 0;
    while (tokens[i].type != -1)
        free((void*)tokens[i++].value);
    i = 0;
    while (split_line[i])
        free(split_line[i++]);
    free(split_line);
    free(tokens);
}

int         display_unclosed_quote_error_message(int *status)
{
    write(2, "Unclosed quotes in input, make sure to match the unclosed quote!\n", 66);
    *status = 2;
    return (0);
}

int         empty_string_and_unclosed_quote_check(const char *read_line, int *status)
{
    int i;
    int quote_type;

    i = 0;
    quote_type = 0;
    while (read_line[i])
    {
        if (is_quote(read_line[i]) && quote_type == 0)
            quote_type = read_line[i++];
        if (read_line[i] == quote_type)
        {
            i++;
            quote_type = 0;
        }
        else
            i++;
    }
    if (quote_type != 0)
        return (display_unclosed_quote_error_message(status));
    else if (*read_line == 0)
        return (0);
    return (1);
}

t_token     *minishell(const char *read_line, t_envs *env_list, int *status)
{
    int     i;
    t_token *tokens;
    char    **split_line;

    i = 0;
    if (!empty_string_and_unclosed_quote_check(read_line, status))
        return (NULL);
    split_line = split_read_line(read_line);
    if (!split_line)
        return (NULL);
    tokens = malloc(sizeof(t_token) * (word_counter(read_line) + 1));
    if (!tokens)
        return (split_clean_up(split_line, word_counter(read_line)), NULL);
    if (!create_token_struct(tokens, split_line))
        return (clean_up_tokens_and_split_line(tokens, split_line), NULL);
    split_clean_up(split_line, word_counter(read_line));
    if (!handle_expansions(env_list, tokens, status))
        return (NULL);
    if (!remove_quotes(tokens))
        return (clean_up_token_and_env_list(tokens, &env_list), NULL);
    syntax_check(tokens, status);
    if (*status == 2)
    	return (free_tokens(tokens), NULL);
    // 	clean_up_token_and_env_list(tokens, &env_list);
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
		free_split(tmp->cmd);
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

int main(int ac, char **av, const char **envp)
{
	char *line;
	t_shell shell;
	t_token *tokens;
    (void)av;

    if(ac != 1)
    {
    	ft_putstr_fd("minishell doesn't take arguments\n", STDERR_FILENO);
     	return (1);
    }
    // line = "$FJ";
    // const char *envp[] = {"BROWSER=/home/guthybarnakoppany/.vscode-server/cli/servers/Stable-618725e67565b290ba4da6fe2d29f8fa1d4e3622/server/bin/helpers/browser.sh",
    // "PATH=/home/guthybarnakoppany/.local/funcheck/host:/home/linuxbrew/.linuxbrew/bin:/home/linuxbrew/.linuxbrew/sbin:/home/guthybarnakoppany/.vscode-server/cli/servers/Stable-618725e67565b290ba4da6fe2d29f8fa1d4e3622/server/bin/remote-cli:/home/guthybarnakoppany/.local/bin:/home/guthybarnakoppany/.local/bin:/opt/orbstack-guest/bin-hiprio:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin:/opt/orbstack-guest/bin:/opt/orbstack-guest/data/bin/cmdlinks:/home/guthybarnakoppany/.local/bin:/home/guthybarnakoppany/.local/bin:/home/guthybarnakoppany/.local/bin:/home/guthybarnakoppany/.local/bin",
    // "DBUS_SESSION_BUS_ADDRESS=unix:path=/run/user/501/bus"
    // "TERM_PROGRAM=vscode", NULL};
	ft_bzero(&shell, sizeof(shell));
	if (!add_envp_to_list(&shell.env_list, envp))
        return (1);
	init_interactive_signals();
	while ((line = readline("minishell$ ")))
	{
		if (line[0] != '\0' || !line)
			add_history(line);
		if (WTERMSIG(g_signal) != 0)
		{
			shell.status = 128 + WTERMSIG(g_signal);
			g_signal = 0;
		}
		tokens = minishell(line, shell.env_list, &shell.status);
		free(line);
		if (!tokens)
			continue;
		shell.cmds = build_cmds(tokens, shell.env_list, &shell);
		free_tokens(tokens);
		if (!shell.cmds)
			continue;
		shell.status = execute_cmds(&shell);
		free_cmds(&shell.cmds);
	}
	// rl_clear_history();
    //tokens = minishell(line, shell.env_list, &shell.status);
    //clean_up_token_and_env_list(tokens, &shell.env_list);
	ft_putstr_fd("exit\n", STDERR_FILENO);
    free_all_and_exit(&shell, shell.status);
}
