/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_quoted.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 16:36:03 by bguthy            #+#    #+#             */
/*   Updated: 2026/09/17 18:14:15 by bguthy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_single_quote(const char letter)
{
	if (letter == SINGLE_QUOTE)
		return (1);
	return (0);
}

int	is_double_quote(const char letter)
{
	if (letter == DOUBLE_QUOTE)
		return (1);
	return (0);
}

int	is_quote(const char letter)
{
	if (is_single_quote(letter) || is_double_quote(letter))
		return (1);
	return (0);
}

int	check_for_quote(const char letter, int *quote_type)
{
	if (letter == SINGLE_QUOTE)
		return (*quote_type = SINGLE_QUOTE, 1);
	else if (letter == DOUBLE_QUOTE)
		return (*quote_type = DOUBLE_QUOTE, 1);
	else
		return (0);
}

void	set_quote_type(int *quote_type, const char letter)
{
	if (is_single_quote(letter) && *quote_type == 0)
		*quote_type = SINGLE_QUOTE;
	if (is_double_quote(letter) && *quote_type == 0)
		*quote_type = DOUBLE_QUOTE;
	if (is_single_quote(letter) && *quote_type == SINGLE_QUOTE)
		*quote_type = 0;
	if (is_double_quote(letter) && *quote_type == DOUBLE_QUOTE)
		*quote_type = 0;
}
