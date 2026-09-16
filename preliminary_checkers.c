/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preliminary_checkers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 17:50:02 by bguthy            #+#    #+#             */
/*   Updated: 2026/09/16 17:53:30 by bguthy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	display_unclosed_quote_error_message(int *status)
{
	write(STDERR_FILENO, "Unclosed quotes ", 16);
	write(STDERR_FILENO, "in input, make ", 15);
	write(STDERR_FILENO, "sure to match ", 14);
	write(STDERR_FILENO, "the unclosed quote!\n", 20);
	*status = 2;
	return (2);
}

int	loop_for_unclosed_quotes(const char *read_line, int *status)
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
	if (loop_for_unclosed_quotes(read_line, status))
		return (display_unclosed_quote_error_message(status));
	return (0);
}

int	preliminary_check(t_token *tokens)
{
	int	token_list_size;

	token_list_size = ft_lstsize(tokens);
	if (token_list_size == 1 && is_redir(tokens[0].value[0]))
	{
		syntax_error_message_display(NULL);
		return (1);
	}
	return (0);
}
