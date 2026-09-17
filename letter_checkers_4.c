/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_assign_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 17:00:28 by bguhty            #+#    #+#             */
/*   Updated: 2026/09/16 16:15:44 by bguthy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_lower_case(char letter)
{
	if (letter >= LOWER_CASE_A && letter <= LOWER_CASE_Z)
		return (1);
	return (0);
}

int	is_upper_case(char letter)
{
	if (letter >= A && letter <= Z)
		return (1);
	return (0);
}

int	is_number(char letter)
{
	if (letter >= '0' && letter <= '9')
		return (1);
	return (0);
}

int	is_underline(char letter)
{
	if (letter == '_')
		return (1);
	return (0);
}
