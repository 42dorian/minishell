/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_executor_helper.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabdulla <dabdulla@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 14:37:31 by dabdulla          #+#    #+#             */
/*   Updated: 2026/09/12 14:37:32 by dabdulla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	restore_io(int saved_stdin, int saved_stdout)
{
	int	return_value;

	return_value = 0;
	if (dup2(saved_stdin, STDIN_FILENO) == -1)
	{
		return_value = 1;
		perror("minishell");
	}
	if (dup2(saved_stdout, STDOUT_FILENO) == -1)
	{
		return_value = 1;
		perror("minishell");
	}
	close(saved_stdin);
	close(saved_stdout);
	return (return_value);
}
