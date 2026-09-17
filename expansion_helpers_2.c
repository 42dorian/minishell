/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_helpers_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 16:56:14 by bguthy            #+#    #+#             */
/*   Updated: 2026/09/17 21:51:11 by bguthy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	how_many_digits(int *number)
{
	int	digits;
	int	local_num;

	digits = 0;
	local_num = *number;
	if (*number == 0)
		return (1);
	if (number < 0)
		digits++;
	while (local_num > 0)
	{
		digits++;
		local_num /= 10;
	}
	return (digits);
}

int	get_len_of_valid_expandable(const char *expandable)
{
	int	len;

	len = 0;
	if (double_dollar_or_question_mark_check(expandable[len]))
		return (1);
	while (expandable[len])
	{
		if (!is_valid_after_dollar_sign(expandable[len]))
			break ;
		len++;
	}
	return (len);
}

char	*get_valid_expandable(const char *expandable)
{
	int		i;
	int		len;
	char	*valid_expandable;

	i = 0;
	len = get_len_of_valid_expandable(expandable);
	valid_expandable = malloc(sizeof(char) * (len + 1));
	if (!valid_expandable)
		return (NULL);
	while (i < len)
	{
		valid_expandable[i] = expandable[i];
		i++;
	}
	valid_expandable[i] = 0;
	return (valid_expandable);
}
