/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_ups.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 17:46:21 by bguthy            #+#    #+#             */
/*   Updated: 2026/09/17 18:09:59 by bguthy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	clean_up_env_list(t_envs **env_list)
{
	ft_lstclear(env_list, free);
	return (0);
}

void	clean_up_tokens_and_split_line(t_token *tokens, char **split_line)
{
	int	i;

	i = 0;
	while (tokens[i].type != -1)
		free((void *)tokens[i++].value);
	i = 0;
	if (split_line)
	{
		while (split_line[i])
			free(split_line[i++]);
	}
	free(split_line);
	free(tokens);
}
