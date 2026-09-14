/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguhty <bguhty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 19:02:10 by bguhty            #+#    #+#             */
/*   Updated: 2026/09/14 11:36:37 by bguhty           ###   ########.fr       */
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
// #include "expansion_without_token_list.c"
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

volatile sig_atomic_t g_signal = 0;

int     determine_quote_type(char letter, int quote_type)
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
        if (set_quote_type(&quote_type, quoted_word[i]))
            i++;
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
    write(STDERR_FILENO, "Unclosed quotes in input, make sure to match the unclosed quote!\n", 65);
    *status = 2;
    return (2);
}

int        loop_for_unclosed_quotes(const char *read_line, int *status)
{
    int quote_type;
    int i;

    i = 0;
    quote_type = 0;
    while (read_line[i])
    {
        if (is_quote(read_line[i]) && quote_type == 0)
            quote_type = read_line[i++];
        if (quote_type == read_line[i])
        {
            quote_type = 0;
            i++;
        }
        else
            i++;
    }
    if (quote_type != 0)
        return (1);
    return (0);
}

int         is_empty_string(const char *read_line, int *status)
{
    if (*read_line == 0)
    {
        *status = 2;
        return (1);
    }
    else
        return (0);
}

int         empty_string_and_unclosed_quote_check(const char *read_line, int *status)
{
    if (is_empty_string(read_line, status))
        return (1);
    if (loop_for_unclosed_quotes(read_line, status));
        return (display_unclosed_quote_error_message(status));
    return (0);
}

t_token     *minishell(const char *read_line, t_envs *env_list, int *status)
{
    int     i;
    t_token *tokens;
    char    **split_line;
    char    *expanded_line;

    i = 0;
    // if (empty_string_and_unclosed_quote_check(read_line, status))
    //     return (NULL);
    expanded_line = handle_expansions_without_token_list(env_list, read_line, status);
    if (!expanded_line)
        return (NULL);
    split_line = split_read_line(expanded_line);
    if (!split_line)
        return (NULL);
    tokens = malloc(sizeof(t_token) * (word_counter(expanded_line) + 1));
    //printf("words: %i\n", word_counter(read_line));
    if (!tokens)
        return (split_clean_up(split_line, word_counter(expanded_line)), NULL);
    if (!create_token_struct(tokens, split_line))
        return (clean_up_tokens_and_split_line(tokens, split_line), NULL);
    split_clean_up(split_line, word_counter(expanded_line));
    // if (!handle_expansions(env_list, tokens, status))
    //     return (NULL);
    if (!remove_quotes(tokens))
        return (clean_up_token_and_env_list(tokens, &env_list), NULL);
    syntax_check(tokens, status);
    return (tokens);
}

int main(int ac, char **av, const char **envp)
{
	char *line;
	t_shell shell;
	t_token *tokens;
    (void)ac;
    (void)av;
    
    // line = malloc(20);
    // line[0] = '"';
    // line[1] = 'e';
    // line[2] = 'x';
    // line[3] = 'p';
    // line[4] = 'o';
    // line[5] = 'r';
    // line[6] = 't';
    // line[7] = 32;
    // line[8] = '"';
    // line[9] = 32;
    // line[10] = 32;
    // line[11] = 32;
    // line[12] = 32;
    // line[13] = 32;
    // line[14] = 32;
    // line[15] = 32;
    // line[16] = 'h';
    // line[17] = 32;
    // line[18] = '"';
    // line[19] = 0;
    // line[0] = '"';
    // line[1] = '\'';
    // line[2] = '$';
    // line[3] = '?';
    // line[4] = '\'';
    // line[5] = '"';
    // line[6] = 0;
    // line = "/bin/echo $USER'$USER'text oui oui     oui  oui $USER oui      $USER ''";
    // const char *envp[] = {"BROWSER=/home/guthybarnakoppany/.vscode-server/cli/servers/Stable-618725e67565b290ba4da6fe2d29f8fa1d4e3622/server/bin/helpers/browser.sh",
    // "PATH=/home/linuxbrew/.linuxbrew/bin:/home/linuxbrew/.linuxbrew/sbin:/home/bguhty/.local/funcheck/host:/home/bguhty/.vscode-server/extensions/vadimcn.vscode-lldb-1.12.2/bin:/home/bguhty/.vscode-server/bin/645f29cc3176500b4b5762ba887cf2a7f0ffdf2c/bin/remote-cli:/home/linuxbrew/.linuxbrew/bin:/home/linuxbrew/.linuxbrew/sbin:/home/bguhty/.local/funcheck/host:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/usr/lib/wsl/lib:/mnt/c/Python314/Scripts/:/mnt/c/Python314/:/mnt/c/Program Files (x86)/NVIDIA Corporation/PhysX/Common:/mnt/c/Program Files (x86)/Razer Chroma SDK/bin:/mnt/c/Program Files/Razer Chroma SDK/bin:/mnt/c/Program Files (x86)/Common Files/Oracle/Java/javapath:/mnt/c/Windows/system32:/mnt/c/Windows:/mnt/c/Windows/System32/Wbem:/mnt/c/Windows/System32/WindowsPowerShell/v1.0/:/mnt/c/Windows/system32/config/systemprofile/AppData/Local/Microsoft/WindowsApps:/mnt/c/WINDOWS/system32:/mnt/c/WINDOWS:/mnt/c/WINDOWS/System32/Wbem:/mnt/c/WINDOWS/System32/WindowsPowerShell/v1.0/:/mnt/c/WINDOWS/System32/OpenSSH/:/mnt/c/Program Files/dotnet/:/mnt/c/Program Files/nodejs/:/mnt/c/ProgramData/chocolatey/bin:/mnt/c/Program Files/Git/cmd:/mnt/c/Users/Computer/AppData/Local/Programs/Python/Python311/Scripts/:/mnt/c/Users/Computer/AppData/Local/Programs/Python/Python311/:/mnt/c/Users/Computer/AppData/Local/Microsoft/WindowsApps:/mnt/c/Users/Computer/AppData/Local/Programs/Microsoft VS Code/bin:/mnt/c/Users/Computer/AppData/Roaming/npm:/snap/bin",
    // "DBUS_SESSION_BUS_ADDRESS=unix:path=/run/user/501/bus"
    // "TERM_PROGRAM=vscode", NULL};
	ft_bzero(&shell, sizeof(shell));
	if (!add_envp_to_list(&shell.env_list, envp))
        return (1);
	init_interactive_signals();
	while ((line = readline("minishell$ ")))
	{
		if (WTERMSIG(g_signal) != 0)
		{
			shell.status = 128 + WTERMSIG(g_signal);
			g_signal = 0;
		}

		if (!ft_strncmp(line, "exitcode", 8))
		{
			printf("%d\n", shell.status);
			continue;
		}
		tokens = minishell(line, shell.env_list, &shell.status);
		if (!tokens || shell.status == 2)
			continue;
		shell.cmds = build_cmds(tokens, shell.env_list);
		if (!shell.cmds)
		{
			shell.status = 1;
			continue;
		}
		shell.status = execute_cmds(&shell);
		if (line[0] != '\0' || !line)
			add_history(line);
	}
    //tokens = minishell(line, shell.env_list, &shell.status);
    //clean_up_token_and_env_list(tokens, &shell.env_list);
	ft_putstr_fd("exit\n", STDOUT_FILENO);
    return (shell.status);
}
