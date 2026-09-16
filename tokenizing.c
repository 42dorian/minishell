/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 17:13:55 by bguhty            #+#    #+#             */
/*   Updated: 2026/09/16 16:09:11 by bguthy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	tokenizer(char *input)
{
	if (string_compare(input, "|"))
		return (token_pipe);
	else if (string_compare(input, ">>"))
		return (token_append);
	else if (string_compare(input, "<<"))
		return (token_heredoc);
	else if (string_compare(input, "<"))
		return (token_redirect_in);
	else if (string_compare(input, ">"))
		return (token_redirect_out);
	else
		return (token_word);
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

int	create_token_struct(t_token *tokens, char **line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		tokens[i].value = normal_copy(line[i]);
		if (!tokens[i].value)
		{
			tokens[i].type = -1;
			return (0);
		}
		tokens[i].type = tokenizer(line[i]);
		tokens[i].quoted = 0;
		i++;
	}
	tokens[i].quoted = -1;
	tokens[i].value = NULL;
	tokens[i].type = -1;
	return (1);
}
