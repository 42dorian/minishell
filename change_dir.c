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

int	change_dir(t_cd *cd, t_cmds *cmd, t_envs **env_list);
int home_dir(t_cd *cd, t_cmds *cmd, t_envs **env_list);
int update_pwd(char *old_pwd, char *pwd, t_cd *cd, t_envs **env_list);

int	cd_bi(t_cmds *cmd, t_envs **env_list)
{
	t_cd 	cd_struct;

	ft_bzero(&cd_struct, sizeof(cd_struct));
	if (!cmd->cmd[1])
	{
		if (home_dir(&cd_struct, cmd, env_list))
			return (1);
		return (0);
	}
	else
		cd_struct.target_path = cmd->cmd[1];
	change_dir(&cd_struct, cmd, env_list);
	return (0);
}

int	change_dir(t_cd *cd, t_cmds *cmd, t_envs **env_list)
{
	if (!getcwd(cd->old_path, sizeof(cd->old_path)))
	{
		print_error(strerror(errno), cmd->cmd[0], STDERR_FILENO);
		return (1);
	}
	if (chdir(cd->target_path) != 0)
	{
		print_error(strerror(errno), cmd->cmd[0], STDERR_FILENO);
		return (1);
	}
	if (!getcwd(cd->new_path, sizeof(cd->new_path)))
	{
		print_error(strerror(errno), cmd->cmd[0], STDERR_FILENO);
		return (1);
	}
	if (update_pwd(cd->old_pwd, cd->pwd, cd, env_list))
		return (1);
	return (0);
}

int update_pwd(char *old_pwd, char *pwd, t_cd *cd, t_envs **env_list)
{
	cd->old_pwd = ft_strdup(old_pwd);
	if (!cd->old_pwd)
		return (1);
	cd->pwd = ft_strdup(pwd);
	if (!cd->pwd)
		return (1);
	cd->value_old_path = ft_strdup(cd->old_path);
	if (!cd->value_old_path)
		return (1);
	cd->value_new_path = ft_strdup(cd->new_path);
	if (!cd->value_new_path)
		return (1);
	if (update_or_add(env_list, cd->value_old_path, cd->old_pwd))
		return (1);
	if (update_or_add(env_list, cd->value_new_path, cd->pwd))
		return (1);
	return (0);
}

int home_dir(t_cd *cd, t_cmds *cmd, t_envs **env_list)
{
	t_envs *tmp;

	tmp = *env_list;
	while(tmp)
	{
		if (ft_strncmp(tmp->value, "HOME", 5) == 0)
		{
			cd->target_path = tmp->value;
			printf("HERE");
		}
		printf("VAL: %s\n",tmp->value);
		tmp = tmp->next;
	}
	if (!getcwd(cd->old_path, sizeof(cd->old_path)))
	{
		print_error(strerror(errno), cmd->cmd[0], STDERR_FILENO);
		return (1);
	}
	if (chdir(cd->target_path) != 0)
	{
		print_error("HOME is not set", cmd->cmd[0], STDERR_FILENO);
		return (1);
	}
	if (!getcwd(cd->new_path, sizeof(cd->new_path)))
	{
		print_error(strerror(errno), cmd->cmd[0], STDERR_FILENO);
		return (1);
	}
	if (update_pwd(cd->old_pwd, "HOME", cd, env_list))
		return (1);
	return (0);
}
