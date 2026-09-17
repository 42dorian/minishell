/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_creation.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 11:41:39 by guthybarnak       #+#    #+#             */
/*   Updated: 2026/09/17 20:10:31 by bguthy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_env_len(const char **envp)
{
	int	len;

	len = 0;
	while (envp[len])
		len++;
	return (len);
}

int	add_envp_to_list(t_envs **my_list, const char **envp)
{
	t_envs	*new_node;
	int		i;
	int		envp_len;

	i = 0;
	envp_len = get_env_len(envp);
	while (i < envp_len)
	{
		new_node = copy_from_envp_to_own_env_list(envp, i);
		if (!new_node)
			return (clean_up_env_list(my_list));
		else
			ft_lstadd_back(my_list, new_node);
		i++;
	}
	if (!put_shlvl_in_env_list(my_list))
		return (0);
	return (1);
}

t_envs	*copy_from_envp_to_own_env_list(const char **envp, int i)
{
	t_envs	*new_node;

	new_node = malloc(sizeof(t_envs));
	if (!new_node)
		return (NULL);
	new_node->key = get_key(envp[i]);
	if (!new_node->key)
		return (free(new_node), NULL);
	new_node->value = get_value(envp[i]);
	if (!new_node->value)
		return (free(new_node->key), free(new_node), NULL);
	new_node->next = NULL;
	return (new_node);
}

int	key_counter(const char *envp)
{
	int	i;

	i = 0;
	while (envp[i] != EQUAL_SIGN)
		i++;
	return (i);
}

int	value_counter(const char *envp)
{
	int	i;
	int	final;

	i = 0;
	while (envp[i] != EQUAL_SIGN)
		i++;
	i++;
	final = i;
	while (envp[i])
		i++;
	return (i - final);
}
