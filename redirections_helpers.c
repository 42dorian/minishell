/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_helpers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabdulla <dabdulla@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/20 16:34:48 by dabdulla          #+#    #+#             */
/*   Updated: 2026/09/20 16:34:49 by dabdulla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_redir_to_back(t_redirs **list, t_redirs *new_redir)
{
	t_redirs	*tmp;

	if (!new_redir || !list)
		return ;
	if (!*list)
	{
		*list = new_redir;
		return ;
	}
	tmp = *list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_redir;
}

void	free_redirs(t_redirs **redirs)
{
	t_redirs	*tmp;
	t_redirs	*next_redir;

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
