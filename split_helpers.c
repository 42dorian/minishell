/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guthybarnakoppany <guthybarnakoppany@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 21:09:28 by bguhty            #+#    #+#             */
/*   Updated: 2026/09/06 19:10:16 by guthybarnak      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int check_for_quote(const char letter, int *quote_type)
{
    if (letter == SINGLE_QUOTE)
        return (*quote_type = SINGLE_QUOTE, 1);
    else if (letter == DOUBLE_QUOTE)
        return (*quote_type = DOUBLE_QUOTE, 1);
    else
        return (0);
}

int     letter_after_dollar_is_num_or_astrisk(const char letter)
{
    if (is_number(letter) || is_astrisk(letter))
        return (1);
    return (0);
}

int word_counter(const char *read_line)
{
    int i;
    int words;

    words = 0;
    i = 0;
    while (read_line[i])
    {
        skip_white_spaces(read_line, &i);
        quote_in_word(read_line, &i, &words);
        skip_white_spaces(read_line, &i);
        if (read_line[i] && is_word_2(read_line, &i, &words))
            words++;
    }
    if (words == 0 && i > 0)
        words++;
    return (words);
}

int     is_special_character(const char *read_line, int i, int *letters)
{
    if (is_heredoc_or_append(read_line[i], read_line[(i) + 1]))
    {
        if (*letters == 0)
            (*letters) += 2;
        return (1);
    }
    else if (is_redir_or_pipe(read_line[i]))
    {
        if (*letters == 0)
            (*letters)++;
        return (1);
    }
    return (0);
}


void    split_clean_up(char **split_line, int i)
{
    int j;

    j = 0;
    while (j < i)
        free(split_line[j++]);
    free(split_line);
    split_line = NULL;
}
