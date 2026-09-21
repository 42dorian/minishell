/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 17:13:55 by bguhty            #+#    #+#             */
/*   Updated: 2026/09/21 12:54:51 by bguthy           ###   ########.fr       */
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

int	add_node_to_initial_token_struct(t_token *tokens, int i, char **line)
{
	tokens[i].value = ft_strdup(line[i]);
	if (!tokens[i].value)
	{
		tokens[i].type = -1;
		return (0);
	}
	tokens[i].type = tokenizer(line[i]);
	if (tokens[i].type == token_heredoc)
		tokens[i + 1].expandable = 0;
	else
		tokens[i + 1].expandable = 1;
	tokens[i].quoted = 0;
	return (0);
}

int	add_first_node_to_initial_token_struct(t_token *tokens, char **line)
{
	tokens[0].value = ft_strdup(line[0]);
	if (!tokens[0].value)
	{
		tokens[0].type = -1;
		return (0);
	}
	tokens[0].type = tokenizer(line[0]);
	if (tokens[0].type == token_heredoc)
		tokens[1].expandable = 0;
	else
		tokens[1].expandable = 1;
	tokens[0].expandable = 1;
	tokens[0].quoted = 0;
	return (0);
}

int	create_token_struct(t_token *tokens, char **line)
{
	int	i;

	i = 1;
	if (add_first_node_to_initial_token_struct(tokens, line))
		return (0);
	while (line[i])
	{
		if (add_node_to_initial_token_struct(tokens, i, line))
			return (0);
		i++;
	}
	tokens[i].quoted = -1;
	tokens[i].value = NULL;
	tokens[i].type = -1;
	return (1);
}
