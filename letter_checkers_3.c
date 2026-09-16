/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   letter_checkers_3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 17:35:58 by bguhty            #+#    #+#             */
/*   Updated: 2026/09/16 17:34:23 by bguthy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_special_character(const char letter1, const char letter2)
{
	if (is_heredoc_or_append(letter1, letter2))
		return (1);
	else if (is_redir_or_pipe(letter1))
		return (1);
	else
		return (0);
}

int	is_white_space(const char letter)
{
	if ((letter <= CARRIAGE_RET && letter >= HORIZONTAL_TAB) || letter == SPACE)
		return (1);
	return (0);
}

int	is_delimeter(const char letter)
{
	if (is_redir_or_pipe(letter))
		return (1);
	else if (is_end(letter))
		return (1);
	else
		return (0);
}
