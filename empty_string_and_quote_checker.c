/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   empty_string_and_quote_checker.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 17:02:03 by bguthy            #+#    #+#             */
/*   Updated: 2026/09/17 22:02:05 by bguthy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	loop_for_unclosed_quotes(const char *read_line)
{
	int	quote_type;
	int	i;

	i = 0;
	quote_type = 0;
	while (read_line[i])
	{
		if (is_quote(read_line[i]) && quote_type == 0)
			quote_type = read_line[i++];
		if (quote_type == read_line[i])
		{
			quote_type = 0;
			i++;
		}
		else
			i++;
	}
	if (quote_type != 0)
		return (1);
	return (0);
}

int	is_empty_string(const char *read_line, int *status)
{
	if (!*read_line)
	{
		*status = 2;
		return (1);
	}
	else
		return (0);
}

int	empty_string_and_unclosed_quote_check(const char *read_line, int *status)
{
	if (is_empty_string(read_line, status))
		return (1);
	if (loop_for_unclosed_quotes(read_line))
		return (display_unclosed_quote_error_message(status));
	return (0);
}
