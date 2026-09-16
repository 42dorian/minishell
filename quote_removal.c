/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_removal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 17:42:49 by bguthy            #+#    #+#             */
/*   Updated: 2026/09/16 17:44:06 by bguthy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	malloc_for_new_word(char **new_word, int len)
{
	*new_word = malloc(sizeof(char) * (len + 1));
	if (!*new_word)
		return (0);
	else
		return (1);
}

char	*get_rid_of_them_quotes(t_token *tokens, int i)
{
	int		j;
	int		quote_type;
	int		local_index;
	char	*new_word;

	quote_type = 0;
	j = 0;
	local_index = 0;
	if (!malloc_for_new_word(&new_word, count_valid_char(tokens[i].value)))
		return (free((void *)tokens[i].value), NULL);
	while (tokens[i].value[j])
	{
		if (check_for_quote(tokens[i].value[j], &quote_type))
		{
			j++;
			while (tokens[i].value[j] != quote_type && tokens[i].value[j])
				new_word[local_index++] = tokens[i].value[j++];
			j++;
		}
		else if (tokens[i].value[j])
			new_word[local_index++] = tokens[i].value[j++];
	}
	new_word[local_index] = 0;
	free((void *)tokens[i].value);
	return (new_word);
}

int	remove_quotes(t_token *tokens)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (tokens[i].value)
	{
		while (tokens[i].value[j])
		{
			if (is_quote(tokens[i].value[j]))
			{
				tokens[i].quoted = 1;
				tokens[i].value = get_rid_of_them_quotes(tokens, i);
				if (!tokens[i].value)
					return (0);
				break ;
			}
			j++;
		}
		i++;
		j = 0;
	}
	return (1);
}
