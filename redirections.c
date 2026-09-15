/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabdulla <dabdulla@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 10:32:32 by dabdulla          #+#    #+#             */
/*   Updated: 2026/09/09 23:49:20 by dabdulla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_in(t_cmds *curr, t_token *tokens, int *i)
{
	if (curr->fd_in != 0)
		close(curr->fd_in);
	curr->fd_in = open(tokens[*i + 1].value, O_RDONLY);
	if (curr->fd_in == -1)
	{
		print_error(strerror(errno), tokens[*i + 1].value, NULL, 2);
		return (1);
	}
	return (0);
}

int	handle_out(t_cmds *curr, t_token *token, int *i)
{
	if (curr->fd_out != 1)
		close(curr->fd_out);
	if (token[*i].type == token_redirect_out)
	{
		curr->fd_out = open(token[*i + 1].value, O_WRONLY | O_CREAT | O_TRUNC,
				0644);
		if (curr->fd_out == -1)
		{
			print_error(strerror(errno), token[*i + 1].value, NULL, 2);
			return (1);
		}
	}
	else if (token[*i].type == token_append)
	{
		curr->fd_out = open(token[*i + 1].value, O_WRONLY | O_CREAT | O_APPEND,
				0644);
		if (curr->fd_out == -1)
		{
			print_error(strerror(errno), token[*i + 1].value, NULL, 2);
			return (1);
		}
	}
	return (0);
}

int	handle_pipe(t_cmds **head, t_cmds **curr)
{
	t_cmds	*next;

	next = NULL;
	next = new_cmd();
	if (!next)
		return (0);
	*head = add_cmd(*head, next);
	if (!*head)
		return (1);
	*curr = next;
	return (0);
}
