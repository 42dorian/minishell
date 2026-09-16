/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   special_characters_checkers.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 17:27:09 by bguhty            #+#    #+#             */
/*   Updated: 2026/09/16 16:17:32 by bguthy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redir_in(const char letter)
{
	if (letter == REDIR_IN)
		return (1);
	return (0);
}

int	is_redir_out(const char letter)
{
	if (letter == REDIR_OUT)
		return (1);
	return (0);
}

int	is_pipe(const char letter)
{
	if (letter == '|')
		return (1);
	return (0);
}

int	is_redir_or_pipe(const char letter)
{
	if (is_redir(letter) || is_pipe(letter))
		return (1);
	return (0);
}

int	is_heredoc_or_append(const char letter1, const char letter2)
{
	if (is_redir_in(letter1) && is_redir_in(letter2))
		return (1);
	else if (is_redir_out(letter1) && is_redir_out(letter2))
		return (1);
	return (0);
}
