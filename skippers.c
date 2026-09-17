/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skippers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 17:44:27 by bguhty            #+#    #+#             */
/*   Updated: 2026/09/17 22:43:57 by bguthy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	skip_white_spaces(const char *read_line, int *i)
{
	while (is_white_space(read_line[*i]))
		(*i)++;
}
