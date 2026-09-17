/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 17:13:55 by bguhty            #+#    #+#             */
/*   Updated: 2026/09/17 13:51:14 by bguthy           ###   ########.fr       */
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

t_new_token	*get_new_node(const char *word, t_envs *env_list)
{
	t_new_token	*new_node;
	
	new_node = malloc(sizeof(t_new_token));
	if (!new_node)
		return (NULL);
	new_node->quoted = 0;
	new_node->value = normal_copy(word);
	if (!new_node->value)
		return (free(new_node), NULL);
	new_node->type = tokenizer(word);
	new_node->env_list = env_list;
	new_node->next = NULL;
	return (new_node);
}

int	len_of_double_pointer(const char **split_line)
{
	int words;

	words = 0;
	while (split_line[words])
		words++;
	return (words);
}

int	create_linked_token_struct(t_new_token **tokens, const char **line, t_envs *env_list)
{
	t_new_token *new_node;
	int		i;
	int		len;
	
	i = 0;
	len = len_of_double_pointer(line);
	while (i < len)
	{
		new_node = get_new_node(line[i], env_list);
		if (!new_node)
			return (0);
		ft_lstadd_back_token(tokens, new_node);
		i++;
	}
	return (1);
}

void	insert_new_node(t_new_token **token_list, int pos, int *status)
{
	int			i;
	t_new_token *current;
	
	i = 0;
	current = *token_list;
	while (i < pos)
	{
		current = current->next;
		i++;
	}
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
		if (string_compare(tokens[i].value, "export"))
			tokens[i].export_flag = 1;
		else
			tokens[i].export_flag = 0;
		tokens[i].type = tokenizer(line[i]);
		tokens[i].quoted = 0;
		i++;
	}
	tokens[i].quoted = -1;
	tokens[i].export_flag = 0;
	tokens[i].value = NULL;
	tokens[i].type = -1;
	return (1);
}
