/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   letter_checkers_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 16:34:00 by bguthy            #+#    #+#             */
/*   Updated: 2026/09/16 17:25:34 by bguthy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_white_space_or_special_character(const char letter)
{
	if (is_white_space(letter) || is_redir_or_pipe(letter))
		return (1);
	return (0);
}

int	solo_standing_special_character(const char *read_line, int *i)
{
	if (is_pipe(read_line[*i]) && check_for_pipe(read_line, i))
		return (1);
	else if (is_redir_in(read_line[*i])
		&& check_for_redirect_in_and_heredoc(read_line, i))
		return (1);
	else if (is_redir_out(read_line[*i])
		&& check_for_redirect_out_and_append(read_line, i))
		return (1);
	return (0);
}

int	is_redir(const char letter)
{
	if (is_redir_in(letter) || is_redir_out(letter))
		return (1);
	return (0);
}

int	letter_after_dollar_is_num_or_astrisk(const char letter)
{
	if (is_number(letter) || is_astrisk(letter))
		return (1);
	return (0);
}

int	is_question_mark(const char letter)
{
	if (letter == QUESTION_MARK)
		return (1);
	return (0);
}
