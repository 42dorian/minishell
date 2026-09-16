/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 17:13:55 by bguhty            #+#    #+#             */
/*   Updated: 2026/09/16 16:41:23 by bguthy           ###   ########.fr       */
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
