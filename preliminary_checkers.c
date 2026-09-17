/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preliminary_checkers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 17:50:02 by bguthy            #+#    #+#             */
/*   Updated: 2026/09/17 18:10:46 by bguthy           ###   ########.fr       */
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

int	is_redir_token(int token_type)
{
	if (token_type == token_append || token_type == token_heredoc)
		return (1);
	else if (token_type == token_redirect_in
		|| token_type == token_redirect_out)
		return (1);
	else
		return (0);
}

int	preliminary_check(t_token *tokens)
{
	int	size;

	size = token_list_size(tokens);
	if (size == 1 && is_redir_token(tokens[0].type))
	{
		syntax_error_message_display(NULL);
		return (1);
	}
	return (0);
}
