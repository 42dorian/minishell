/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   final_token_construct.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/20 20:10:30 by bguthy            #+#    #+#             */
/*   Updated: 2026/09/21 18:49:31 by bguthy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_last_node_to_final_token_list(t_token *final_token)
{
	final_token->value = NULL;
	final_token->quoted = -1;
	final_token->expandable = -1;
	final_token->type = token_invalid;
}

t_token	*alloc_for_final_token_struct(t_token *initial_token_list,
		t_envs *env_list, int *exit_code)
{
	t_token	*final_token_list;
	int		struct_len;
	int		total_len;

	struct_len = token_list_size(initial_token_list);
	total_len = get_len_of_total_token_struct(initial_token_list, env_list,
			exit_code, struct_len);
	if (total_len == -1)
		return (NULL);
	final_token_list = ft_calloc(sizeof(t_token), total_len + 1);
	if (!final_token_list)
		return (NULL);
	return (final_token_list);
}

void	set_prev_and_local_index_to_zero(int *prev_index, int *local_index)
{
	*prev_index = 0;
	*local_index = 0;
}

int	can_expand(char *word, int expandable)
{
	if (dollar_in_word(word) && expandable)
		return (1);
	return (0);
}

t_token	*create_final_token_struct(t_token *tokens, t_envs *env_list,
		int *exit_code)
{
	int		prev_i;
	int		local_i;
	char	**exp_split;
	t_token	*new_token;

	set_prev_and_local_index_to_zero(&prev_i, &local_i);
	new_token = alloc_for_final_token_struct(tokens, env_list, exit_code);
	if (!new_token)
		return (NULL);
	while (tokens[prev_i].type != token_invalid)
	{
		if (can_expand(tokens[prev_i].value, tokens[prev_i].expandable))
		{
			exp_split = split_token(tokens[prev_i].value, env_list, exit_code);
			if (!exp_split)
				return (NULL);
			if (!add_to_final_struct(new_token, &local_i, exp_split))
				return (split_clean_up(exp_split), NULL);
			split_clean_up(exp_split);
		}
		else if (!cp_t_node(&new_token[local_i], tokens[prev_i], &local_i))
			return (NULL);
		prev_i++;
	}
	return (add_last_node_to_final_token_list(&new_token[local_i]), new_token);
}
