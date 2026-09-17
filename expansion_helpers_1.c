/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_helpers_1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 12:53:31 by guthybarnak       #+#    #+#             */
/*   Updated: 2026/09/17 21:49:45 by bguthy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	dollar_in_word(const char *word)
{
	int	i;

	i = 0;
	while (word[i])
	{
		if (is_dollar_sign(word[i]))
			return (1);
		i++;
	}
	return (0);
}

char	*get_from_my_env_list(const char *expandable, t_envs env_list)
{
	t_envs	*next_one;

	while (env_list.key)
	{
		if (string_compare(expandable, env_list.key))
			return (normal_copy(env_list.value));
		if (!env_list.next)
			break ;
		next_one = env_list.next;
		env_list = *next_one;
	}
	return (ft_strdup(""));
}

int	get_len_of_real_env(const char *test_env, t_envs *env_list)
{
	char	*real_env;
	int		len;

	real_env = get_from_my_env_list(test_env, *env_list);
	if (!real_env)
		len = -1;
	else
		len = ft_strlen(real_env);
	return (free(real_env), len);
}

int	get_len_of_current_expandable(const char *expandable, t_envs *env_list,
		int *exit_code)
{
	char	*test_env;
	char	*real_env;
	int		len;

	test_env = get_valid_expandable(expandable);
	if (!test_env)
		return (-1);
	else if (string_compare(test_env, "?"))
		return (free(test_env), how_many_digits(exit_code));
	len = get_len_of_real_env(test_env, env_list);
	if (len == -1)
		return (free(test_env), -1);
	else
		return (free(test_env), len);
}
