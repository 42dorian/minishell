/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stepping_in_input.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 20:34:44 by bguhty            #+#    #+#             */
/*   Updated: 2026/09/17 22:42:45 by bguthy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_for_special_character(const char *read_line, int *i, int *words)
{
	if (*i > 0 && is_pipe(read_line[*i]) && !is_delimeter(read_line[(*i) - 1]))
		return ((*words) += 2, (*i)++, 1);
	else if (is_pipe(read_line[*i]))
		return ((*words)++, (*i)++, 1);
	else if (*i > 0 && is_heredoc_or_append(read_line[*i], read_line[(*i) + 1])
		&& !is_delimeter(read_line[(*i) - 1]))
		return ((*words) += 2, (*i) += 2, 1);
	else if (is_heredoc_or_append(read_line[*i], read_line[(*i) + 1]))
		return ((*words)++, (*i) += 2, 1);
	else if (*i > 0 && is_redir(read_line[*i]) && !is_delimeter(read_line[(*i)
				- 1]))
		return ((*words) += 2, (*i)++, 1);
	else if (is_redir(read_line[*i]))
		return ((*words)++, (*i)++, 1);
	else
		return (0);
}
