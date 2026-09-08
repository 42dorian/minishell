/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_handler_helpers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabdulla <dabdulla@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 11:26:00 by dabdulla          #+#    #+#             */
/*   Updated: 2026/09/07 22:56:53 by dabdulla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_path(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (i);
		i++;
	}
	return (0);
}

int	check_access(char *cmd)
{
	int			status;
	struct stat	path_stat;

	status = 0;
	if (is_dir(cmd, &path_stat))
	{
		status = 1;
		return (status);
	}
	if (access(cmd, F_OK) == 0)
	{
		status = 2;
		if (access(cmd, X_OK) == 0)
			status = 3;
	}
	return (status);
}



int	is_dir(char *cmd, struct stat *path_stat)
{
	if (stat(cmd, path_stat) == 0)
	{
		if ((path_stat->st_mode & S_IFMT) == S_IFDIR)
			return (1);
	}
	return (0);
}
