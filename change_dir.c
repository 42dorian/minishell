/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chdir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabdulla <dabdulla@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 09:49:38 by dabdulla          #+#    #+#             */
/*   Updated: 2026/09/05 22:25:00 by dabdulla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int change_dir(t_cmds *cmd, t_envs **env_list)
{
	char *target_path;
	char old_path[PATH_MAX];
	char new_path[PATH_MAX];

	if (!cmd->cmd[1])
	{
		// go to home or print error
	}
	else
		target_path = cmd->cmd[1];
	if (!getcwd(old_path, sizeof(old_path)))
	{
		print_error(strerror(errno), cmd->cmd[0], STDERR_FILENO);
		return (1);
	}
	if (chdir(target_path) != 0)
	{
		print_error(strerror(errno), cmd->cmd[0], STDERR_FILENO);
		return (1);
	}
	if (!getcwd(new_path, sizeof(new_path)))
	{
		print_error(strerror(errno), cmd->cmd[0], STDERR_FILENO);
		return (1);
	}
	char *old_pwd = ft_strdup("OLDPWD");//failcheck
	char *pwd = ft_strdup("PWD");//failcheck
	char *value_old_path= ft_strdup(old_path);
	char *value_new_path = ft_strdup(new_path);

	update_or_add(env_list, value_old_path, old_pwd);
	update_or_add(env_list, value_new_path, pwd);
	return (0);
}
