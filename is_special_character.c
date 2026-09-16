/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_special_character.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 17:35:58 by bguhty            #+#    #+#             */
/*   Updated: 2026/09/16 16:17:16 by bguthy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_terminator(const char letter)
{
	if (letter == '\0')
		return (1);
	return (0);
}

int	is_astrisk(const char letter)
{
	if (letter == '*')
		return (1);
	return (0);
}
