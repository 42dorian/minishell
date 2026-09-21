/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 14:02:51 by bguthy            #+#    #+#             */
/*   Updated: 2026/09/21 15:40:19 by bguthy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	len_of_split_line(char **read_line)
{
	int	len;

	len = 0;
	if (!read_line)
		return (len);
	if (read_line[0][0] == 0)
		len++;
	while (read_line[len])
		len++;
	return (len);
}

char	**allocating_double_pointer(const char *read_line)
{
	int		words;
	char	**split_line;

	words = word_counter(read_line);
	if (read_line[0] == 0)
		words++;
	split_line = malloc(sizeof(char *) * (words + 1));
	return (split_line);
}

int	fill_up_double_pointer(char **split_line, const char *read_line)
{
	int	i;
	int	w;

	i = 0;
	w = 0;
	if (read_line[i] == 0)
	{
		split_line[w++] = ft_strdup("");
		if (!split_line[w - 1])
			return (0);
	}
	while (read_line[i])
	{
		skip_white_spaces(read_line, &i);
		if (read_line[i])
			split_line[w++] = copy_till_next_word(read_line, &i);
		if (w > 0 && split_line[w - 1] == NULL)
			return (split_clean_up(split_line), 0);
	}
	split_line[w] = NULL;
	return (1);
}

char	**split_read_line(char *read_line)
{
	char	**split_line;

	split_line = allocating_double_pointer(read_line);
	if (!split_line)
		return (NULL);
	if (!fill_up_double_pointer(split_line, read_line))
		return (NULL);
	else
		return (split_line);
}
