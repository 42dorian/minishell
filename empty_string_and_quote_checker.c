/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   empty_string_and_quote_checker.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 17:02:03 by bguthy            #+#    #+#             */
/*   Updated: 2026/09/21 19:31:22 by bguthy           ###   ########.fr       */
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
		if (quote_type == 0 && is_quote(read_line[i]))
			quote_type = read_line[i];
		else if (quote_type == read_line[i])
			quote_type = 0;
		i++;
	}
	if (quote_type != 0)
		return (1);
	return (0);
}

int	is_empty_string(const char *read_line)
{
	if (!*read_line)
		return (1);
	else
		return (0);
}

int	empty_string_and_unclosed_quote_check(char *read_line, int *status)
{
	if (is_empty_string(read_line))
		return (free(read_line), 1);
	if (loop_for_unclosed_quotes(read_line))
		return (free(read_line), display_unclosed_quote_error_message(status));
	return (0);
}
