/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   permissions.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabdulla <dabdulla@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 18:53:03 by dabdulla          #+#    #+#             */
/*   Updated: 2026/09/07 23:01:30 by dabdulla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_status(int status, char *cmd);
static char	*format_path(char *cmd_name, char *path);
static void assign_exit_status(int status, int *exit_status);

char	*handling_path(char *cmd_name, char *path, int *exit_status)
{
	char	**split_path;
	char	*cmd_path;
	int		status;

	status = 0;
	if (ft_strncmp(cmd_name, "", 1) == 0)
	{
		print_error("command not found", cmd_name, NULL, STDERR_FILENO);
		*exit_status = 1;
		return (NULL);
	}
	if (ft_strchr(cmd_name, '/'))
	{
		status = check_access(cmd_name);
		cmd_path = ft_strdup(cmd_name);
		if (!cmd_path)
			return (NULL);
		if (status == 3)
			return (cmd_path);
		assign_exit_status(status, exit_status);
		return (print_status(status, cmd_name), free(cmd_path), NULL);
	}
	split_path = ft_split(path, ':');
	if (!split_path)
		return (NULL);
	cmd_path = find_cmd_path(cmd_name, split_path, &status);
	free_split(split_path);
	if (!cmd_path)
		return (print_status(status, cmd_name), assign_exit_status(status, exit_status), NULL);
	return (cmd_path);
}

void assign_exit_status(int status, int *exit_status)
{
	if (status == 1 || status == 2)
		*exit_status = 126;
	else
		*exit_status = 127;
}

void	free_split(char **strs)
{
	int	i;

	if (!strs)
		return ;
	i = 0;
	while (strs[i])
	{
		free(strs[i]);
		i++;
	}
	free(strs);
}

static char	*format_path(char *cmd_name, char *path)
{
	char	*cmd_path;
	char	*tmp;

	tmp = ft_strjoin(path, "/");
	if (!tmp)
		return (NULL);
	cmd_path = ft_strjoin(tmp, cmd_name);
	if (!cmd_path)
		return (free(tmp), NULL);
	return (cmd_path);
}

char	*find_cmd_path(char *cmd_name, char **split_path, int *status)
{
	char	*cmd_path;
	int		i;
	int		curr_status;

	curr_status = 0;
	i = 0;
	cmd_path = NULL;
	while (split_path[i])
	{
		cmd_path = format_path(cmd_name, split_path[i]);
		if (!cmd_path)
			return (NULL);
		curr_status = check_access(cmd_path);
		if (curr_status > *status)
			*status = curr_status;
		if (*status == 3)
			return (cmd_path);
		i++;
		free(cmd_path);
	}
	return (NULL);
}

static void	print_status(int status, char *cmd)
{
	if (status == 0)
	{
		if (ft_strchr(cmd, '/'))
			print_error("No such file or directory", cmd, NULL, 2);
		else
			print_error("command not found", cmd, NULL, 2);
	}
	else if (status == 1)
		print_error("is a directory", cmd, NULL, 2);
	else if (status == 2)
		print_error("Permission denied", cmd, NULL, 2);
}
