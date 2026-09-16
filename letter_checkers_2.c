/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   letter_checkers_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 17:28:05 by bguthy            #+#    #+#             */
/*   Updated: 2026/09/16 17:29:50 by bguthy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_end(const char letter)
{
	if (is_white_space(letter) || is_terminator(letter))
		return (1);
	return (0);
}

int	double_dollar_or_question_mark_check(const char letter)
{
	if (is_dollar_sign(letter) || is_question_mark(letter))
		return (1);
	else
		return (0);
}

int	is_valid_after_dollar_sign(const char letter)
{
	if (is_number(letter) || is_upper_case(letter) || is_lower_case(letter)
		|| is_underline(letter))
		return (1);
	return (0);
}

int	is_terminator(const char letter)
{
	if (letter == '\0')
		return (1);
	return (0);
}

int	is_astrisk(const char letter)
{
	if (letter == '*')
		return (1);
	return (0);
}
