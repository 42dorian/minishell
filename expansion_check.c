/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_check.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guthybarnakoppany <guthybarnakoppany@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 12:53:31 by guthybarnak       #+#    #+#             */
/*   Updated: 2026/09/06 18:46:06 by guthybarnak      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int     dollar_in_word(const char *word)
{
    int i;

    i = 0;
    while (word[i])
    {
        if (is_dollar_sign(word[i]))
            return (1);
        i++;
    }
    return (0);
}

int     get_pid_len()
{
    pid_t   pid;
    int     len;

    pid = getpid();
    len = digit_counter(pid);
    return (len);
}

int     digit_counter(pid_t pid)
{
    int digits;

    digits = 0;
    while (pid > 0)
    {
        pid /= 10;
        digits++;
    }
    return (digits);
}

char    *convert_pid_to_string()
{
    pid_t   pid;
    int     digits;
    char    *pid_string;

    pid = getpid();
    pid = (int)pid;
    digits = digit_counter(pid);
    pid_string = malloc(sizeof(char) * (digits + 1));
    if (!pid_string)
        return (NULL);
    pid_string[digits] = 0;
    while (pid > 0)
    {
        pid_string[--digits] = pid % 10 + '0';
        pid /= 10;
    }
    return (pid_string);
}

int    handle_expansions(t_envs *env_list, t_token *tokens, int exit_code)
{
    int i;
    int len;

    i = 0;
    len = 0;
    while (tokens[i].value)
    {
        if (dollar_in_word(tokens[i].value))
        {
            len = get_full_len_of_expandable(tokens[i], env_list, tokens, exit_code);
            if (len == -1)
                return (clean_up_token_and_env_list(tokens, &env_list));
            tokens[i].value = get_full_expandable_word(tokens[i], env_list, len, tokens[0].type);
        }
        i++;
    }
    return (1);
}

int     is_valid_after_dollar_sign(const char letter)
{
    if (!is_number(letter) && !is_upper_case(letter) && !is_lower_case(letter) && !is_underline(letter))
        return (0);
    return (1);
}

int     get_len_of_valid_expandable(const char *expandable)
{
    int len;

    len = 0;
    while (expandable[len])
    {

        if (is_dollar_sign(expandable[len]) || is_question_mark(expandable[len]))
        {
            len++;
            break ;
        }
        if (!is_valid_after_dollar_sign(expandable[len]))
            break ;
        len++;
    }
    return (len);
}

char    *get_valid_expandable(const char *expandable)
{
    int     i;
    int     len;
    char    *valid_expandable;

    i = 0;
    len = get_len_of_valid_expandable(expandable);
    valid_expandable = malloc(sizeof(char) * (len + 1));
    if (!valid_expandable)
        return (NULL);
    while (i < len)
    {
        valid_expandable[i] = expandable[i];
        i++;
    }
    valid_expandable[i] = 0;
    return (valid_expandable);
}

char    *get_from_my_env_list(const char *expandable, t_envs *env_list)
{
    while (env_list && env_list->key)
    {
        if (string_compare(expandable, env_list->key))
            return (env_list->value);
    }
    return (NULL);
}

int     how_many_digits(int number)
{
    int digits;

    if (number == 0)
        return (1);
    if (number < 0)
        digits = 1;
    else
        digits = 0;
    while (number > 0)
    {
        digits++;
        number /= 10;
    }
    return (digits);
}

int     get_len_of_real_env(const char *test_env, t_envs *env_list)
{
    char    *real_env;
    int     len;

    real_env = getenv(test_env);
    if (!real_env)
        real_env = get_from_my_env_list(test_env, env_list);
    if (!real_env)
        len = -1;
    else
        len = ft_strlen(real_env);
    return (len);
}

int     get_len_of_current_expandable(const char *expandable, t_envs *env_list, int exit_code)
{
    char    *test_env;
    char    *real_env;
    int     len;

    test_env = get_valid_expandable(expandable);
    len = get_len_of_real_env(test_env, env_list);
    if (len == -1)
        return (free(test_env), -1);
    if (string_compare(test_env, "$"))
        return (get_pid_len());
    else if (string_compare(test_env, "?"))
        return (how_many_digits(exit_code));
    else
        return (len);
}

int     count_valid_characters_after_dollar_sign(const char *curr_expandable)
{
    int i;

    i = 1;
    while (curr_expandable[i])
    {
        if (is_dollar_sign(curr_expandable[i]) || is_question_mark(curr_expandable[i]))
        {
            i++;
            break ;
        }
        if (!is_valid_after_dollar_sign(curr_expandable[i]))
            break ;
        i++;
    }
    return (i);
}

int     is_end(const char letter)
{
    if (is_white_space(letter) || letter == 0)
        return (1);
    return (0);
}

int     get_full_len_of_expandable(t_token curr_token, t_envs *env_list, t_token *tokens, int exit_code)
{
    int     i;
    int     total_len;
    int     curr_len;
    int     single_quote_counter;

    single_quote_counter = 0;
    i = 0;
    curr_len = 0;
    total_len;
    while (curr_token.value[i])
    {
        if (is_single_quote(curr_token.value[i]))
            single_quote_counter++;
        if (is_dollar_sign(curr_token.value[i]) && !is_end(curr_token.value[i + 1]) && single_quote_counter % 2 == 0)
        {
            curr_len = get_len_of_current_expandable(&curr_token.value[i + 1], env_list, exit_code);
            if (curr_len == -1)
                return (-1);
            else
                total_len += curr_len;
            i += (count_valid_characters_after_dollar_sign(&curr_token.value[i]));
            curr_len = 0;
        }
        else
        {
            total_len++;
            i++;
        }
    }
    return (total_len);
}

void    cat_to_fully_expanded(char *fully_expanded, const char new_letter)
{
    int index;

    index = ft_strlen(fully_expanded);
    fully_expanded[index] = new_letter;
}

char    *ft_itoa(int number)
{
    char    *converted;
    int     num_dup;
    int     i;

    i = 0;
    converted = malloc(sizeof(char) * (how_many_digits(number) + 1));
    if (!converted)
        return (NULL);
    if (number < 0)
    {
        converted[i++] = '-';
        number = -number;
    }
    num_dup = number;
    while (i < how_many_digits(number))
    {
        converted[i++] = num_dup % 10 + '0';
        num_dup /= 10;
    }
    return (converted);
}

void    make_expansion(char *fully_expnaded, const char *mock_expand, t_envs *env_list, int exit_code)
{
    int     expand_index;
    char    *test_env;
    int     env_index;

    env_index = 0;
    expand_index = ft_strlen(fully_expnaded);
    test_env = getenv(mock_expand);
    if (!test_env)
        test_env = get_from_my_env_list(mock_expand, env_list);
    if (string_compare(mock_expand, "$"))
        test_env = convert_pid_to_string();
    else if (string_compare(mock_expand, "?"))
        test_env = ft_itoa(exit_code);
    if (test_env)
    {
        while (test_env[env_index])
        {
            fully_expnaded[expand_index] = test_env[env_index];
            expand_index++;
            env_index++;
        }
    }
}

char    *get_full_expandable_word(t_token curr_token, t_envs *env_list, int len, int exit_code)
{
    char    *fully_expanded;
    char    *mock_expand;
    int quote_counter;
    int i;

    i = 0;
    quote_counter = 0;
    fully_expanded = ft_calloc(sizeof(char), (len + 1));
    if (!fully_expanded)
        return (NULL);
    while (curr_token.value[i])
    {
        if (is_quote(curr_token.value[i]))
            quote_counter++;
        if (is_dollar_sign(curr_token.value[i]) && !is_end(curr_token.value[i + 1]) && quote_counter % 2 == 0)
        {
            mock_expand = get_valid_expandable(curr_token.value + i + 1);
            make_expansion(fully_expanded, mock_expand, env_list, exit_code);
            i += (ft_strlen(mock_expand) + 1);
        }
        else
            cat_to_fully_expanded(fully_expanded, curr_token.value[i++]);
    }
    fully_expanded[len] = 0;
    printf("expansion successfull:\n %s\n", fully_expanded);
    return (fully_expanded);
}
