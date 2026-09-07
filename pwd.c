/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabdulla <dabdulla@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 10:43:33 by dabdulla          #+#    #+#             */
/*   Updated: 2026/09/05 18:22:58 by dabdulla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pwd(void)
{
	char	buffer[PATH_MAX];

	if (!getcwd(buffer, sizeof(buffer)))
	{
		print_error(strerror(errno), "pwd", STDERR_FILENO);
		return (1);
	}
	ft_putstr_fd(buffer, STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	return (0);
}
