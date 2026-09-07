/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guthybarnakoppany <guthybarnakoppany@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 14:02:51 by bguthy            #+#    #+#             */
/*   Updated: 2026/09/06 19:11:43 by guthybarnak      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int     is_single_quote(const char letter)
{
    if (letter == SINGLE_QUOTE)
        return (1);
    return (0);
}

int     is_double_quote(const char letter)
{
    if (letter == DOUBLE_QUOTE)
        return (1);
    return (0);
}

int     is_quote(const char letter)
{
    if (is_single_quote(letter) || is_double_quote(letter))
        return (1);
    return (0);
}

int     count_letters_till_next_quote(const char *read_line, int *i)
{
    int letters;
    int quote;
    
    quote = read_line[*i];
    letters = 0;
    *i += 1;
    while (read_line[*i])
    {
        if (read_line[(*i)] == quote)
            break ;
        letters++;
        (*i)++;
    }
    return (letters + 1);
}

int     is_compatible_with_dollar_sign(const char letter)
{
    if (is_quote(letter))
        return (0);
    if (is_dollar_sign(letter))
        return (0);
    if (is_white_space_or_special_character(letter))
        return (0);
    return (1);
}

int     count_letters_for_dollar_sign(const char *read_line, int *i)
{
    int letters;
    
    letters = 1;
    *i += 1;
    while (read_line[*i])
    {
        if (is_dollar_after_dollar(read_line[(*i)]) || is_question_mark(read_line[(*i)]))
        {
            letters++;
            (*i)++;
            break ;
        }
        if (is_compatible_with_dollar_sign(read_line[*i]))
        {
            (*i)++;
            letters++;
        }
        else
            break ;
    }
    return (letters);
}

int     count_letters_till_next_word(const char *read_line, int i)
{
    int letters;
    
    letters = 0;
    while (read_line[i])
    {
        if (is_dollar_sign(read_line[i]))
            letters += count_letters_for_dollar_sign(read_line, &i);
        if (is_quote(read_line[i]))
            letters+= count_letters_till_next_quote(read_line, &i);
        if (is_white_space(read_line[i]) || !read_line[i])
            return (letters);
        if (is_special_character(read_line, i, &letters))
            return (letters);
        i++;
        letters++;
    }
    return (letters);
}

char    *copy_till_next_word(const char *read_line, int *i)
{
    char    *new_word;
    int     local_index;
    int     letters;
    
    local_index = 0;
    letters = count_letters_till_next_word(read_line, *i);
    new_word = malloc(sizeof(char) * (letters + 1));
    if (!new_word)
        return (NULL);
    while (!is_white_space(read_line[*i]) && read_line[(*i)] && local_index < letters)
    {
        new_word[local_index] = read_line[(*i)];
        if (is_quote(new_word[local_index]))
            copy_till_next_quote(read_line, i, new_word, &local_index);
        if ((is_heredoc_or_append(read_line[*i], read_line[(*i) + 1]) && local_index >= letters))
             break ;
         else if (local_index >= letters && is_redir_or_pipe(read_line[*i]) && (!is_redir_or_pipe(new_word[local_index - 1]) || is_redir_or_pipe(new_word[local_index - 1])))
             break ;
        if (!read_line[*i])
            break ;
        local_index++;
        (*i)++;
    }
    new_word[local_index] = 0;
    return (new_word);
}

void    copy_till_next_quote(const char *read_line, int *i, char *new_word, int *new_index)
{
    int     quote_type;

    quote_type = read_line[*i];
    (*i)++;
    (*new_index)++;
    while (read_line[*i])
    {
        new_word[*new_index] = read_line[*i];
        if ((new_word[*new_index]) == quote_type)
            return ;
        (*new_index)++;
        (*i)++;
    }
    return ;
}

char    **allocating_double_pointer(const char *read_line)
{
    int     words;
    char    **split_line;

    words = word_counter(read_line);
    split_line = malloc(sizeof(char *) * (words + 2));
    return (split_line);
}

int    fill_up_double_pointer(char **split_line, const char *read_line)
{
    int i;
    int w;

    i = 0;
    w = 0;
    while (read_line[i])
    {
        skip_white_spaces(read_line, &i);
        if (!is_white_space(read_line[i]) && read_line[i])
            split_line[w++] = copy_till_next_word(read_line, &i);
        if (split_line[w - 1] == NULL)
            return (split_clean_up(split_line, w), 0);
    }
    split_line[w] = NULL;
    return (1);
}

char    **split_read_line(const char *read_line)
{
    char    **split_line;
    split_line = allocating_double_pointer(read_line);
    if (!split_line)
        return (NULL);
    if (!fill_up_double_pointer(split_line, read_line))
        return (NULL);
    else
        return (split_line);
}
