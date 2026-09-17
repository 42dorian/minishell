/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_removal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 17:42:49 by bguthy            #+#    #+#             */
/*   Updated: 2026/09/17 14:07:49 by bguthy           ###   ########.fr       */
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

char	*get_rid_of_them_quotes_linked_list(t_new_token *tokens)
{
	int		i;
	int		quote_type;
	int		local_index;
	char	*new_word;

	quote_type = 0;
	i = 0;
	local_index = 0;
	if (!malloc_for_new_word(&new_word, count_valid_char(tokens->value)))
		return (free((void *)tokens->value), NULL);
	while (tokens->value[i])
	{
		if (check_for_quote(tokens->value[i], &quote_type))
		{
			i++;
			while (tokens->value[i] != quote_type && tokens->value[i])
				new_word[local_index++] = tokens->value[i++];
			i++;
		}
		else if (tokens->value[i])
			new_word[local_index++] = tokens->value[i++];
	}
	new_word[local_index] = 0;
	free((void *)tokens->value);
	return (new_word);
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

int	eligible_for_quote_removal(const char letter, int export_flag)
{
	if (is_quote(letter) && !export_flag)
		return (1);
	return (0);
}

int	remove_quotes_linked_list(t_new_token *tokens)
{
	int	i;

	i = 0;
	while (tokens)
	{
		while (tokens->value[i])
		{
			if (is_quote(tokens->value[i]))
			//if ((i == 0 && is_quote(tokens[i].value[j])) || (i > 0 && eligible_for_quote_removal(tokens[i].value[j], tokens[i - 1].export_flag)))
			{
				tokens->quoted = 1;
				tokens->value = get_rid_of_them_quotes_linked_list(tokens);
				if (!tokens->value)
					return (0);
				break ;
			}
			i++;
		}
		i = 0;
		tokens = tokens->next;
	}
	return (1);
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
			//if (is_quote(tokens[i].value[j]))
			if ((i == 0 && is_quote(tokens[i].value[j])) || (i > 0 && eligible_for_quote_removal(tokens[i].value[j], tokens[i - 1].export_flag)))
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
