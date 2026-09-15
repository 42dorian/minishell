/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_without_token_list.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguhty <bguhty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 20:32:29 by guthybarnak       #+#    #+#             */
/*   Updated: 2026/09/15 10:01:26 by bguhty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int     within_quotes(int quote_type)
{
    if (quote_type != 0)
        return (1);
    else
        return (0);
}

int     is_dollar_or_question_mark(const char letter)
{
    if (is_dollar_sign(letter) || is_question_mark(letter))
        return (1);
    else
        return (0);
}

int     eligible_for_expansion(const char *read_line, int i, int single_quote_counter)
{
    if (is_dollar_sign(read_line[i]) && !is_end(read_line[i + 1] && single_quote_counter % 2 == 0) && (is_valid_after_dollar_sign(read_line[i + 1]) || is_dollar_or_question_mark(read_line[i + 1])))
        return (1);
    else
        return (0);
}

int     get_full_len_of_expandable_without_token_list(const char *read_line, t_envs *env_list, int *exit_code)
{
    int     i;
    int     total_len;
    int     curr_len;
    int     single_quote_counter;
    int     quote_flag;

    single_quote_counter = 0;
    quote_flag = 0;
    i = 0;
    curr_len = 0;
    total_len = 0;
    while (read_line[i])
    {
        set_quote_flag_and_count_single_quotes(&quote_flag, &single_quote_counter, read_line[i]);
        if (eligible_for_expansion(read_line, i, single_quote_counter))
        {
            curr_len = get_len_of_current_expandable(&read_line[i + 1], env_list, exit_code);
            if (curr_len == 0)
            {
                total_len++;
                i++;
            }
            if (curr_len == -1)
                return (-1);
            else
                total_len += curr_len;
            i += (count_valid_characters_after_dollar_sign(&read_line[i]));
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

char    *get_full_expandable_word_without_token_list(const char *read_line, t_envs *env_list, int len, int *exit_code)
{
    char    *fully_expanded;
    char    *mock_expand;
    int     single_quote_counter;
    int     i;
    int     quote_flag;

    quote_flag = 0;
    i = 0;
    single_quote_counter = 0;
    fully_expanded = ft_calloc(sizeof(char), (len + 1));
    if (!fully_expanded)
        return (NULL);
    while (read_line[i])
    {
        set_quote_flag_and_count_single_quotes(&quote_flag, &single_quote_counter, read_line[i]);
        if (eligible_for_expansion(read_line, i, single_quote_counter))
        {
            mock_expand = get_valid_expandable(read_line + i + 1);
            make_expansion(fully_expanded, mock_expand, env_list, exit_code);
            i += (ft_strlen(mock_expand) + 1);
        }
        else
            cat_to_fully_expanded(fully_expanded, read_line[i++]);
    }
    fully_expanded[len] = 0;
    return (fully_expanded);
}

char    *handle_expansions_without_token_list(t_envs *env_list, const char *read_line, int *exit_code)
{
    int i;
    int len;
    char *new_word;

    if (!dollar_in_word(read_line))
        return ((char *)read_line);
    i = 0;
    len = get_full_len_of_expandable_without_token_list(read_line, env_list, exit_code);
    if (len == -1)
    {
        clean_up_env_list(&env_list);
        return (NULL);
    }
    new_word = get_full_expandable_word_without_token_list(read_line, env_list, len, exit_code);
    if (!new_word)
        return (NULL);
    else
        return (new_word);
}
