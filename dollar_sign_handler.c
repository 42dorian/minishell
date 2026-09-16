/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_sign_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 17:33:26 by bguhty            #+#    #+#             */
/*   Updated: 2026/09/16 18:16:21 by bguthy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_after_dollar_sign(const char *read_line, int *i, int *words)
{
	while (read_line[*i] && !is_white_space(read_line[*i]))
	{
		if (is_heredoc_or_append(read_line[*i], read_line[(*i) + 1]))
		{
			(*i) += 2;
			(*words) += 2;
			return ;
		}
		else if (is_redir_or_pipe(read_line[*i]))
		{
			(*i)++;
			(*words) += 2;
			return ;
		}
		(*i)++;
	}
}

int	is_dollar_sign(const char letter)
{
	if (letter == DOLLAR_SIGN)
		return (1);
	return (0);
}

int	dollar_is_standing_alone(const char letter)
{
	if (is_white_space(letter) || is_terminator(letter))
		return (1);
	return (0);
}

int	is_question_mark_after_dollar(const char letter)
{
	if (letter == QUESTION_MARK)
		return (1);
	return (0);
}

int	dollar_sign_exception(const char *read_line, int *i, int *words)
{
	if (is_dollar_sign(read_line[*i]))
	{
		if (double_dollar_or_question_mark_check(read_line[(*i) + 1]))
			(*i) += 2;
		else if (dollar_is_standing_alone(read_line[(*i) + 1]))
		{
			(*words)++;
			(*i)++;
			return (1);
		}
		else if (letter_after_dollar_is_num_or_astrisk(read_line[(*i) + 1]))
			(*i) += 2;
		else
			process_after_dollar_sign(read_line, i, words);
		if (is_end(read_line[*i]))
			(*words)++;
		return (1);
	}
	return (0);
}
