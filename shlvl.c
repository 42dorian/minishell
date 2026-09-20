/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shlvl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 19:48:57 by bguthy            #+#    #+#             */
/*   Updated: 2026/09/20 22:16:40 by bguthy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	in_env_list(t_envs **my_list, char *missing_key)
{
	t_envs	*curr;

	curr = *my_list;
	while (curr)
	{
		if (string_compare(curr->key, missing_key))
			return (1);
		curr = curr->next;
	}
	return (0);
}

char	*extract_value_from_env_list(t_envs **my_list, const char *missing_key)
{
	t_envs	*curr;

	curr = *my_list;
	while (curr)
	{
		if (string_compare(curr->key, missing_key))
			return (ft_strdup(curr->value));
		curr = curr->next;
	}
	return (ft_strdup(""));
}

char	*allocate_one(void)
{
	char	*one;

	one = malloc(sizeof(char) * 2);
	if (!one)
		return (NULL);
	one[0] = '1';
	one[1] = 0;
	return (one);
}

char	*get_string_value(t_envs **my_list, char *literal_shlvl)
{
	char	*string_value;
	int		num_value;

	string_value = extract_value_from_env_list(my_list, literal_shlvl);
	if (!string_value)
		return (NULL);
	num_value = ft_atoi(string_value);
	num_value++;
	free(string_value);
	string_value = ft_itoa(&num_value);
	return (string_value);
}

int	put_shlvl_in_env_list(t_envs **my_list)
{
	char	*string_value;
	char	*literal_shlvl;
	char	*one;

	one = allocate_one();
	if (!one)
		return (0);
	literal_shlvl = ft_strdup("SHLVL");
	if (!literal_shlvl)
		return (free(one), 0);
	if (in_env_list(my_list, literal_shlvl))
	{
		string_value = get_string_value(my_list, literal_shlvl);
		if (!string_value)
			return (free(literal_shlvl), free(one), 0);
		if (!update_or_add(my_list, string_value, literal_shlvl))
			return (free(one), 1);
	}
	else if (!update_or_add(my_list, one, literal_shlvl))
		return (1);
	return (0);
}
