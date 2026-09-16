/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabdulla <dabdulla@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 14:38:46 by dabdulla          #+#    #+#             */
/*   Updated: 2026/09/16 14:38:49 by dabdulla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_env(char *key, char *value, t_envs **env_list)
{
	t_envs	*new;

	new = ft_calloc(1, sizeof(t_envs));
	if (!new)
		return (1);
	new->key = key;
	new->value = value;
	ft_lstadd_back(env_list, new);
	return (0);
}
