/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copying.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 16:40:55 by bguthy            #+#    #+#             */
/*   Updated: 2026/09/17 22:53:16 by bguthy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*copy_till_next_word(const char *read_line, int *i)
{
	char	*new_word;
	int		local_index;
	int		letters;

	local_index = 0;
	letters = count_letters_till_next_word(read_line, *i);
	new_word = malloc(sizeof(char) * (letters + 1));
	if (!new_word)
		return (NULL);
	while (local_index < letters)
		new_word[local_index++] = read_line[(*i)++];
	new_word[local_index] = 0;
	return (new_word);
}

void	copy_till_next_quote(const char *read_line, int *i, char *new_word,
		int *new_index)
{
	int	quote_type;

	quote_type = read_line[*i];
	(*i)++;
	(*new_index)++;
	while (read_line[*i])
	{
		new_word[*new_index] = read_line[*i];
		if ((new_word[*new_index]) == quote_type)
			return ;
		(*new_index)++;
		(*i)++;
	}
	return ;
}

char	*normal_copy(const char *get_copied)
{
	int		i;
	char	*new_word;

	i = 0;
	new_word = malloc(sizeof(char) * (ft_strlen(get_copied) + 1));
	if (!new_word)
		return (NULL);
	while (i < ft_strlen(get_copied))
	{
		new_word[i] = get_copied[i];
		i++;
	}
	new_word[i] = 0;
	return (new_word);
}
