/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_count_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguhty <bguhty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 17:46:11 by bguhty            #+#    #+#             */
/*   Updated: 2026/09/10 14:54:35 by bguhty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int check_for_pipe(const char *read_line, int *i)
{
    if (*i == 0 && is_end(read_line[1]))
        return ((*i)++, 1);
    else if (read_line[(*i) + 1] == '\0' && *i != 0 && is_white_space_or_special_character(read_line[(*i) - 1]))
        return ((*i)++, 1);
    else if (*i != 0 && is_white_space(read_line[(*i) - 1]) && is_white_space_or_special_character(read_line[(*i) + 1]))
        return ((*i)++, 1);
    else
        return (0);
}

int check_for_redirect_in_and_heredoc(const char *read_line, int *i)
{
    if (*i == 0 && is_end(read_line[1]))
        return ((*i)++, 1);
    else if (*i == 0 && read_line[1] == REDIR_IN && is_end(read_line[2]))
        return ((*i) += 2, 1);
    else if (*i != 0 && is_white_space(read_line[(*i) - 1]) && is_end(read_line[(*i) + 1]))
        return ((*i)++, 1);
    else if (*i != 0 && is_white_space(read_line[(*i) - 1]) && read_line[(*i) + 1] == REDIR_IN && is_end(read_line[(*i) + 2]))
        return ((*i) += 2, 1);
    return (0);
}

int check_for_redirect_out_and_append(const char *read_line, int *i)
{
    if (*i == 0 && is_end(read_line[1]))
        return ((*i)++, 1);
    else if (*i == 0 && read_line[1] == REDIR_OUT && is_end(read_line[2]))
        return ((*i) += 2, 1);
    else if (*i != 0 && is_white_space(read_line[(*i) - 1]) && is_white_space(read_line[(*i) + 1]))
        return ((*i)++, 1);
    else if (*i != 0 && is_white_space(read_line[(*i) - 1]) && read_line[(*i) + 1] == REDIR_OUT && is_end(read_line[(*i) + 2]))
        return ((*i) += 2, 1);
    return (0);
}
