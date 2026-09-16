/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_value_extraction.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 17:43:29 by bguthy            #+#    #+#             */
/*   Updated: 2026/09/16 17:43:51 by bguthy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	copy_key(const char *envp, char *new_key)
{
	int	i;

	i = 0;
	while (envp[i] != EQUAL_SIGN)
	{
		new_key[i] = envp[i];
		i++;
	}
	new_key[i] = 0;
}

char	*insert_key(const char *envp)
{
	char	*new_key;

	new_key = malloc(sizeof(char) * (key_counter(envp) + 1));
	if (!new_key)
		return (NULL);
	copy_key(envp, new_key);
	return (new_key);
}

void	copy_value(const char *envp, char *new_value)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (envp[i] != EQUAL_SIGN)
		i++;
	i++;
	while (envp[i])
		new_value[j++] = envp[i++];
	new_value[j] = 0;
}

char	*insert_value(const char *envp)
{
	char	*new_value;

	new_value = malloc(sizeof(char) * (value_counter(envp) + 1));
	if (!new_value)
		return (NULL);
	copy_value(envp, new_value);
	return (new_value);
}
