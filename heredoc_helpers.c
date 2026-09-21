/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabdulla <dabdulla@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 13:46:16 by dabdulla          #+#    #+#             */
/*   Updated: 2026/09/16 13:46:19 by dabdulla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_heredoc_warning(const char *eof)
{
	ft_putstr_fd("minishell: warning: ", STDOUT_FILENO);
	ft_putstr_fd("here-document delimited by end-of-file (wanted '",
		STDOUT_FILENO);
	ft_putstr_fd((char *)eof, STDOUT_FILENO);
	ft_putstr_fd("')\n", STDOUT_FILENO);
}

char	*expanded_line(char *line, t_envs *env)
{
	int		len;
	char	*expanded;
	int		exit_code;

	exit_code = 0;
	len = get_full_len_of_expandable(line, env, &exit_code);
	if (len == -1)
		return (NULL);
	expanded = get_full_expandable_word(line, env, len, &exit_code);
	return (expanded);
}
