/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabdulla <dabdulla@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 11:22:08 by dabdulla          #+#    #+#             */
/*   Updated: 2026/09/09 23:58:19 by dabdulla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmds	*build_cmds(t_token *tokens, t_envs *env, t_shell *shell)
{
	t_cmds	*head;
	t_cmds	*curr;
	int		i;
	int		process_token_val;

	process_token_val = 0;
	i = 0;
	curr = new_cmd();
	if (!curr)
		return (NULL);
	head = curr;
	while (tokens[i].value != NULL)
	{
		if (tokens[i].type == token_word)
		{
			if (!add_arg_to_cmd(curr, tokens[i].value))
				return (free_cmd(head), NULL);
		}
		process_token_val = process_token(&head, &curr, tokens, &i, env);
		if (process_token_val != 0)
		{
			shell->status = process_token_val;
			return (free_cmd(head), NULL);
		}
		i++;
	}
	return (head);
}

int	process_token(t_cmds **head, t_cmds **curr, t_token *t, int *i, t_envs *env)
{
	int	status;

	status = 0;
	if (t[*i].type == token_heredoc)
	{
		status = handle_heredoc(*curr, t, i, env);
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
		status = handle_pipe(head, curr);
	return (status);
}
