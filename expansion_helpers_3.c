/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_helpers_3.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 17:14:52 by bguthy            #+#    #+#             */
/*   Updated: 2026/09/21 15:23:14 by bguthy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	eligible_for_expansion(const char *read_line, int i, int quote_flag)
{
	if (is_dollar_sign(read_line[i]) && !is_end(read_line[i + 1])
		&& quote_flag != 1 && (is_valid_after_dollar_sign(read_line[i + 1])
			|| is_question_mark(read_line[i + 1])))
		return (1);
	else
		return (0);
}

int	check_res_of_curr_len_and_increment_accordingly(int *curr_len,
		int *total_len, int *i, const char *read_line)
{
	if (*curr_len == -1)
		return (1);
	else if (*curr_len == 0)
		(*total_len)++;
	else
		*total_len += *curr_len;
	move_index_and_set_curr_len_to_zero(read_line, i, curr_len);
	return (0);
}

void	move_index_and_set_curr_len_to_zero(const char *read_line, int *i,
		int *curr_len)
{
	*i += count_valid_characters_after_dollar_sign(&read_line[*i]);
	*curr_len = 0;
}

void	increment_total_len_and_index_by_one(int *total_len, int *i)
{
	(*total_len)++;
	(*i)++;
}

void	set_curr_and_total_len_to_zero(int *curr_len, int *total_len)
{
	*curr_len = 0;
	*total_len = 0;
}
