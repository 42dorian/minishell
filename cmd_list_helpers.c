/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_list_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabdulla <dabdulla@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 15:55:37 by dabdulla          #+#    #+#             */
/*   Updated: 2026/09/02 16:14:50 by dabdulla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_cmd(t_cmds *cmds)
{
	t_cmds	*curr;
	t_cmds	*next;

	if (!cmds)
		return ;
	curr = cmds;
	while (curr)
	{
		next = curr->next;
		free_split(curr->cmd);
		free(curr);
		curr = next;
	}
	curr = NULL;
}

t_cmds	*new_cmd(void)
{
	t_cmds	*new;

	new = ft_calloc(1, sizeof(t_cmds));
	if (!new)
		return (NULL);
	new->fd_out = 1;
	return (new);
}

t_cmds	*add_cmd(t_cmds *head, t_cmds *new_list)
{
	t_cmds	*tmp;

	if (!new_list)
		return (NULL);
	if (!head)
	{
		head = new_list;
		return (head);
	}
	tmp = head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_list;
	return (head);
}

int	add_arg_to_cmd(t_cmds *node, const char *arg)
{
	char	**tmp;
	int		count;
	int		i;

	if (!node)
		return (0);
	count = 0;
	i = -1;
	while (node->cmd && node->cmd[count])
		count++;
	tmp = ft_calloc(count + 2, sizeof(char *));
	if (!tmp)
		return (0);
	while (++i < count)
		tmp[i] = node->cmd[i];
	tmp[i] = ft_strdup(arg);
	if (!tmp[i])
		return (free(tmp), 0);
	free(node->cmd);
	node->cmd = tmp;
	return (1);
}
