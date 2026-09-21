/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabdulla <dabdulla@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 11:22:08 by dabdulla          #+#    #+#             */
/*   Updated: 2026/09/12 12:48:23 by dabdulla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	err_check(int value, t_shell *shell);

t_cmds	*build_cmds(t_token *t, t_shell *shell)
{
	t_cmds	*head;
	t_cmds	*curr;
	int		i;

	i = -1;
	curr = new_cmd();
	if (!curr)
		return (NULL);
	head = curr;
	while (t[++i].value != NULL)
	{
		if (t[i].type == token_word && !add_arg_to_cmd(curr, t[i].value))
			return (free_cmds(&head), NULL);
		if (err_check(process_token(&curr, t, &i, shell), shell))
			return (free_cmds(&head), NULL);
	}
	return (head);
}

static int	err_check(int value, t_shell *shell)
{
	if (!value)
		return (0);
	if (value != 0)
	{
		shell->status = value;
		if (value == 130)
			return (1);
	}
	return (0);
}

int	process_token(t_cmds **curr, t_token *t, int *i, t_shell *shell)
{
	int	status;

	status = 0;
	if (t[*i].type == token_heredoc)
	{
		status = handle_heredoc(*curr, t, i, shell);
		(*i)++;
	}
	else if (t[*i].type == token_redirect_in)
	{
		status = handle_in(*curr, t, i);
		(*i)++;
	}
	else if (t[*i].type == token_redirect_out || t[*i].type == token_append)
	{
		status = handle_out(*curr, t, i);
		(*i)++;
	}
	else if (t[*i].type == token_pipe)
		status = handle_pipe(curr);
	return (status);
}
