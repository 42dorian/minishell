/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_list_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabdulla <dabdulla@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 15:55:37 by dabdulla          #+#    #+#             */
/*   Updated: 2026/09/24 00:03:03 by dabdulla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmds	*new_cmd(void)
{
	t_cmds	*new;

	new = ft_calloc(1, sizeof(t_cmds));
	if (!new)
		return (NULL);
	new->fd_out = 1;
	return (new);
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
