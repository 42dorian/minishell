/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_letters.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 16:36:59 by bguthy            #+#    #+#             */
/*   Updated: 2026/09/16 17:41:52 by bguthy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_letters_till_next_quote(const char *read_line, int *i)
{
	int	letters;
	int	quote;

	quote = read_line[*i];
	letters = 0;
	*i += 1;
	while (read_line[*i])
	{
		if (read_line[(*i)] == quote)
			break ;
		letters++;
		(*i)++;
	}
	return (letters + 1);
}

int	count_letters_for_dollar_sign(const char *read_line, int *i)
{
	int	letters;

	letters = 1;
	*i += 1;
	while (read_line[*i])
	{
		if (is_dollar_sign(read_line[(*i)])
			&& double_dollar_or_question_mark_check(read_line[(*i) + 1]))
		{
			letters++;
			(*i)++;
			break ;
		}
		if (is_valid_after_dollar_sign(read_line[*i]) || is_quote(read_line[*i])
			|| is_dollar_sign(read_line[*i]))
		{
			(*i)++;
			letters++;
		}
		else
			break ;
	}
	return (letters);
}

int	count_letters_till_next_word(const char *read_line, int i)
{
	int	letters;

	letters = 0;
	while (read_line[i])
	{
		if (is_dollar_sign(read_line[i]))
			letters += count_letters_for_dollar_sign(read_line, &i);
		if (is_quote(read_line[i]))
			letters += count_letters_till_next_quote(read_line, &i);
		if (is_white_space(read_line[i]) || !read_line[i])
			return (letters);
		if (count_letters_on_special_character(read_line, i, &letters))
			return (letters);
		i++;
		letters++;
	}
	return (letters);
}

int	count_valid_characters_after_dollar_sign(const char *curr_expandable)
{
	int	i;

	i = 1;
	while (curr_expandable[i])
	{
		if ((is_dollar_sign(curr_expandable[i])
				|| is_question_mark(curr_expandable[i])) && i == 0)
		{
			i++;
			break ;
		}
		if (!is_valid_after_dollar_sign(curr_expandable[i]))
			break ;
		i++;
	}
	return (i);
}

int	count_valid_char(const char *quoted_word)
{
	int	i;
	int	counter;
	int	quote_type;

	quote_type = 0;
	counter = 0;
	i = 0;
	while (quoted_word[i])
	{
		set_quote_type(&quote_type, quoted_word[i]);
		if (quote_type != quoted_word[i])
		{
			counter++;
			i++;
		}
		else
			i++;
	}
	return (counter);
}
