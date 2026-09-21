/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   final_token_construct_helpers.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 17:22:59 by bguthy            #+#    #+#             */
/*   Updated: 2026/09/21 18:27:13 by bguthy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_if_need_free(char *exp_token)
{
	if (!exp_token)
		return (-1);
	else
		free(exp_token);
	return (-1);
}

int	get_len_of_total_token_struct(t_token *tokens, t_envs *env_list,
		int *exit_code, int struct_len)
{
	int		i;
	int		len;
	char	*exp_token;
	char	**split_token;

	i = 0;
	len = 0;
	while (i < struct_len)
	{
		if (dollar_in_word(tokens[i].value))
		{
			exp_token = handle_expansions(env_list, tokens[i].value, exit_code);
			split_token = split_read_line(exp_token);
			if (!split_token)
				return (check_if_need_free(exp_token));
			len += len_of_split_line(split_token);
			free(exp_token);
			split_clean_up(split_token);
		}
		else
			len++;
		i++;
	}
	return (len);
}

char	**split_token(char *line, t_envs *env_list, int *exit_code)
{
	char	*expanded_line;
	char	**split_expanded;

	expanded_line = handle_expansions(env_list, line, exit_code);
	if (!expanded_line)
		return (NULL);
	split_expanded = split_read_line(expanded_line);
	if (!split_expanded)
		return (free(expanded_line), NULL);
	return (free(expanded_line), split_expanded);
}

int	add_to_final_struct(t_token *full_token, int *i, char **expanded_split)
{
	int	split_len;
	int	split_index;
	int	counter;

	split_index = 0;
	counter = 0;
	split_len = len_of_split_line(expanded_split);
	while (counter < split_len)
	{
		full_token[*i].value = ft_strdup(expanded_split[split_index]);
		if (!full_token[*i].value)
			return (0);
		full_token[*i].type = token_word;
		full_token[*i].quoted = 0;
		(*i)++;
		split_index++;
		counter++;
	}
	return (1);
}
