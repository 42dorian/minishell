/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stepping_in_input.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguhty <bguhty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 20:34:44 by bguhty            #+#    #+#             */
/*   Updated: 2026/09/10 14:54:58 by bguhty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int     is_white_space(const char letter)
{
    if ((letter <= CARRIAGE_RET && letter >= HORIZONTAL_TAB) || letter == SPACE)
        return (1);
    return (0);
}

int     quote_in_word(const char *read_line, int *i, int *words)
{
    int quote_type;
    
    quote_type = 0;
    if (check_for_quote(read_line[*i], &quote_type))
    {
        skip_to_next_quote(read_line, i, quote_type);
        skip_white_spaces(read_line, i);
        (*words)++;
        return (1);
    }
    return (0);
}

int     is_white_space_or_special_character(const char letter)
{
    if (is_white_space(letter) || is_redir_or_pipe(letter))
        return (1);
    return (0);
}

int     solo_standing_special_character(const char *read_line, int *i)
{
    if (is_pipe(read_line[*i]) && check_for_pipe(read_line, i))
        return (1);
    else if (is_redir_in(read_line[*i]) && check_for_redirect_in_and_heredoc(read_line, i))
        return (1);
    else if (is_redir_out(read_line[*i])&& check_for_redirect_out_and_append(read_line, i))
        return (1);
    return (0);
}

int     is_redir(const char letter)
{
    if (is_redir_in(letter) || is_redir_out(letter))
        return (1);
    return (0);
}

int     is_delimeter(const char letter)
{
    if (is_redir_or_pipe(letter))
        return (1);
    else if (is_white_space(letter))
        return (1);
    else
        return (0);
}

int     check_for_special_character(const char *read_line, int *i, int *words)
{
    if (*i > 0 && is_pipe(read_line[*i]) && !is_delimeter(read_line[(*i) - 1]))
        return ((*words) += 2, (*i)++, 1);
    else if (is_pipe(read_line[*i]))
        return ((*words)++, (*i)++, 1);
    else if (*i > 0 && is_heredoc_or_append(read_line[*i], read_line[(*i) + 1]) && !is_delimeter(read_line[(*i) - 1]))
        return ((*words) += 2, (*i) += 2, 1);
    else if (is_heredoc_or_append(read_line[*i], read_line[(*i) + 1]))
        return ((*words)++, (*i) += 2, 1);
    else if (*i > 0 && is_redir(read_line[*i]) && !is_delimeter(read_line[(*i) - 1]))
        return ((*words) += 2, (*i)++, 1);
    else if (is_redir(read_line[*i]))
        return ((*words)++, (*i)++, 1);
    else
        return (0);
}

int     is_word(const char *read_line, int *i, int *words)
{
    while (!is_white_space(read_line[*i]) && read_line[*i])
    {
        if (solo_standing_special_character(read_line, i))
            return (1);
        else if (dollar_sign_exception(read_line, i, words))
            return (0);
        else if (quote_in_word(read_line, i, words))
            return (0);
        else if (check_for_special_character(read_line, i, words))
            return (0);
        (*i)++;
    }
    return (1);
}
