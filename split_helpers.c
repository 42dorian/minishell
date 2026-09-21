/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 21:09:28 by bguhty            #+#    #+#             */
/*   Updated: 2026/09/21 14:06:42 by bguthy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	skip_to_next_word(const char *read_line, int *i, int *words)
{
	if (check_for_special_character(read_line, i, words))
		(*words)--;
	skip_white_spaces(read_line, i);
	(*words)++;
}

void	set_quote_flag(int *quote_flag, const char letter)
{
	if (is_single_quote(letter) && *quote_flag == 1)
		*quote_flag = 0;
	else if (is_single_quote(letter) && *quote_flag == 0)
		*quote_flag = 1;
	else if (is_double_quote(letter) && *quote_flag == 2)
		*quote_flag = 0;
	else if (is_double_quote(letter) && *quote_flag == 0)
		*quote_flag = 2;
}

int	word_counter(const char *read_line)
{
	int	words;
	int	i;
	int	quote_flag;

	words = 0;
	i = 0;
	quote_flag = 0;
	while (read_line[i])
	{
		set_quote_flag(&quote_flag, read_line[i]);
		if (is_delimeter(read_line[i]) && quote_flag == 0)
			skip_to_next_word(read_line, &i, &words);
		else
			i++;
	}
	if (i > 0 && !is_delimeter(read_line[i - 1]))
		words++;
	return (words);
}

int	count_letters_on_special_character(const char *read_line, int i,
		int *letters)
{
	if (is_heredoc_or_append(read_line[i], read_line[(i) + 1]))
	{
		if (*letters == 0)
			(*letters) += 2;
		return (1);
	}
	else if (is_redir_or_pipe(read_line[i]))
	{
		if (*letters == 0)
			(*letters)++;
		return (1);
	}
	return (0);
}

void	split_clean_up(char **split_line)
{
	int	i;

	i = 0;
	while (split_line[i])
	{
		free(split_line[i]);
		i++;
	}
	free(split_line);
	split_line = NULL;
}
