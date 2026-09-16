/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rest_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 10:26:24 by bguhty            #+#    #+#             */
/*   Updated: 2026/09/16 16:09:44 by bguthy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	string_compare(const char *string1, const char *string2)
{
	int	i;

	i = 0;
	if (!string1 || !string2)
		return (0);
	while (string1[i] || string2[i])
	{
		if (string1[i] != string2[i])
			return (0);
		i++;
	}
	if (string1[i] == '\0' && string2[i] == '\0')
		return (1);
	return (0);
}
