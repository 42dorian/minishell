/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skippers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguhty <bguhty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 17:44:27 by bguhty            #+#    #+#             */
/*   Updated: 2026/09/14 14:52:14 by bguhty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void skip_white_spaces(const char *read_line, int *i)
{
    while (is_white_space(read_line[*i]))
        (*i)++;
}

void skip_to_next_quote(const char *read_line, int *i, char quote_type)
{
    while (read_line[*i])
    {
        (*i)++;
        if (read_line[*i] == quote_type)
        {
            (*i)++;
            break ;
        }
    }
    //skip_non_white_spaces(read_line, i);
}

void     skip_non_white_spaces(const char *read_line, int *i)
{
    while (!is_white_space(read_line[*i]) && read_line[*i])
    {
        if (is_redir_or_pipe(read_line[*i]))
            break ;
        (*i)++;
    }
}

