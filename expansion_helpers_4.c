/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_helpers_4.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 16:31:11 by bguthy            #+#    #+#             */
/*   Updated: 2026/09/16 17:38:38 by bguthy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cat_to_fully_expanded(char *fully_expanded, const char new_letter)
{
	int	index;

	index = ft_strlen(fully_expanded);
	fully_expanded[index] = new_letter;
}

void	cat_test_env_to_fully_expanded(char *test_env, char *fully_expanded)
{
	int	env_index;
	int	expand_index;

	env_index = 0;
	expand_index = ft_strlen(fully_expanded);
	while (test_env[env_index])
		fully_expanded[expand_index++] = test_env[env_index++];
	free(test_env);
}

void	make_expansion(char *fully_expnaded, const char *mock_expand,
		t_envs *env_list, int *exit_code)
{
	char	*test_env;
	int		env_index;

	env_index = 0;
	test_env = NULL;
	test_env = get_from_my_env_list(mock_expand, *env_list);
	if (string_compare(mock_expand, "$"))
	{
		free(test_env);
		test_env = convert_pid_to_string();
	}
	else if (string_compare(mock_expand, "?"))
	{
		free(test_env);
		test_env = ft_itoa(exit_code);
	}
	if (test_env)
		cat_test_env_to_fully_expanded(test_env, fully_expnaded);
}

void	set_quote_flag_and_index_to_zero(int *quote_flag, int *i)
{
	*quote_flag = 0;
	*i = 0;
}
