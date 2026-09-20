/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling_expansions.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 20:32:29 by guthybarnak       #+#    #+#             */
/*   Updated: 2026/09/20 22:16:29 by bguthy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_full_len_of_expandable(const char *read_line, t_envs *env_list,
		int *exit_code)
{
	int	i;
	int	total_len;
	int	curr_len;
	int	quote_flag;

	set_quote_flag_and_index_to_zero(&quote_flag, &i);
	set_curr_and_total_len_to_zero(&curr_len, &total_len);
	while (read_line[i])
	{
		set_quote_flag(&quote_flag, read_line[i]);
		if (eligible_for_expansion(read_line, i, quote_flag))
		{
			curr_len = get_len_of_current_expandable(&read_line[i + 1],
					env_list, exit_code);
			if (check_res_of_curr_len_and_increment_accordingly(&curr_len,
					&total_len, &i, read_line))
				return (-1);
		}
		else
			increment_total_len_and_index_by_one(&total_len, &i);
	}
	return (total_len);
}

char	*get_full_expandable_word(const char *read_line, t_envs *env_list,
		int len, int *exit_code)
{
	char	*fully_expanded;
	char	*valid_expandable;
	int		i;
	int		quote_flag;

	set_quote_flag_and_index_to_zero(&quote_flag, &i);
	fully_expanded = ft_calloc(sizeof(char), (len + 1));
	if (!fully_expanded)
		return (NULL);
	while (read_line[i])
	{
		set_quote_flag(&quote_flag, read_line[i]);
		if (eligible_for_expansion(read_line, i, quote_flag))
		{
			valid_expandable = get_valid_expandable(read_line + i + 1);
			make_expansion(fully_expanded, valid_expandable, env_list,
				exit_code);
			i += (ft_strlen(valid_expandable) + 1);
			free(valid_expandable);
		}
		else
			cat_to_fully_expanded(fully_expanded, read_line[i++]);
	}
	fully_expanded[len] = 0;
	return (fully_expanded);
}

char	*handle_expansions(t_envs *env_list, const char *read_line,
		int *exit_code)
{
	int		i;
	int		len;
	char	*new_word;

	i = 0;
	if (!dollar_in_word(read_line))
		return (ft_strdup(read_line));
	len = get_full_len_of_expandable(read_line, env_list, exit_code);
	if (len == -1)
	{
		clean_up_env_list(&env_list);
		return (NULL);
	}
	new_word = get_full_expandable_word(read_line, env_list, len, exit_code);
	if (!new_word)
		return (NULL);
	else
		return (new_word);
}
