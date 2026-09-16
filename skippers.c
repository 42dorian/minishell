/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skippers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 17:44:27 by bguhty            #+#    #+#             */
/*   Updated: 2026/09/16 16:17:24 by bguthy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	skip_white_spaces(const char *read_line, int *i)
{
	while (is_white_space(read_line[*i]))
		(*i)++;
}

void	skip_to_next_quote(const char *read_line, int *i, char quote_type)
{
	while (read_line[*i])
	{
		(*i)++;
		if (read_line[*i] == quote_type)
		{
			(*i)++;
			break ;
		}
	}
}

void	skip_non_white_spaces(const char *read_line, int *i)
{
	while (!is_white_space(read_line[*i]) && read_line[*i])
	{
		if (is_redir_or_pipe(read_line[*i]))
			break ;
		(*i)++;
	}
}
