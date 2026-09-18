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

void add_redir_to_back(t_redirs **list, t_redirs *new_redir)
{
	t_redirs *tmp;
	if (!new_redir || !list)
		return ;
	if (!*list)
	{
		*list = new_redir;
		return ;
	}
	tmp = *list;
	while(tmp->next)
		tmp = tmp->next;
	tmp->next = new_redir;
}

void free_redirs(t_redirs **redirs)
{
	t_redirs *tmp;
	t_redirs *next_redir;

	if (!redirs || !*redirs)
		return ;
	tmp = *redirs;
	while (tmp)
	{
		next_redir = tmp->next;
		if (tmp->filename)
			free(tmp->filename);
		free(tmp);
		tmp = next_redir;
	}
	*redirs = NULL;
}

int	handle_in(t_cmds *curr, t_token *tokens, int *i)
{
	t_redirs *new_redir;

	new_redir = ft_calloc(1, sizeof(t_redirs));
	if (!new_redir)
		return (1);
	new_redir->filename = ft_strdup(tokens[*i +1].value);
	if (!new_redir->filename)
		return (free_redirs(&curr->redirs), free(new_redir), 1);
	new_redir->flags = O_RDONLY;
	new_redir->is_out = 0;
	new_redir->next = NULL;
	add_redir_to_back(&curr->redirs, new_redir);
	return (0);
}

int	handle_out(t_cmds *curr, t_token *token, int *i)
{
	t_redirs *new_redir;

	new_redir = ft_calloc(1, sizeof(t_redirs));
	if (!new_redir)
		return (1);
	new_redir->filename = ft_strdup(token[*i + 1].value);
	if (!new_redir->filename)
		return (free_redirs(&curr->redirs), free(new_redir), 1);
	new_redir->is_out = 1;
	new_redir->next = NULL;
	if (token[*i].type == token_redirect_out)
		new_redir->flags = O_WRONLY | O_CREAT | O_TRUNC;
	else if (token[*i].type == token_append)
		new_redir->flags = O_WRONLY | O_CREAT | O_APPEND;
	add_redir_to_back(&curr->redirs, new_redir);
	return (0);
}

int	handle_pipe(t_cmds **curr)
{
	t_cmds	*next;

	next = new_cmd();
	if (!next)
		return (1);
	next->prev = *curr;
	(*curr)->next = next;
	*curr = next;
	return (0);
}
