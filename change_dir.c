/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chdir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabdulla <dabdulla@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 09:49:38 by dabdulla          #+#    #+#             */
/*   Updated: 2026/09/07 22:45:46 by dabdulla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	change_dir(t_cd *cd, t_cmds *cmd, t_envs **env_list);
int home_dir(t_cd *cd, t_envs **env_list);
int update_pwd(char *old_pwd, char *pwd, t_envs **env_list);
int old_dir(t_cd *cd, t_envs **env_list);

int	cd_bi(t_cmds *cmd, t_envs **env_list)
{
	t_cd 	cd_struct;

	ft_bzero(&cd_struct, sizeof(cd_struct));
	if (cmd->cmd[1] != NULL && cmd->cmd[2] != NULL)
		return (print_error("too many arguments", cmd->cmd[0], STDERR_FILENO), 2);
	else if (!cmd->cmd[1])
	{
		if (home_dir(&cd_struct, env_list))
			return (print_error("HOME not set", cmd->cmd[0], STDERR_FILENO), 1);
	}
	else if (ft_strncmp(cmd->cmd[1], "-", 2) == 0)
	{
		if (old_dir(&cd_struct, env_list))
			return (print_error("OLDPWD not set", cmd->cmd[0], STDERR_FILENO), 1);
	}
	else
		cd_struct.target_path = cmd->cmd[1];
	if (change_dir(&cd_struct, cmd, env_list))
		return (1);
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
	if (update_pwd(cd->old_path, cd->new_path, env_list))
		return (1);
	return (0);
}

int update_pwd(char *old_pwd, char *pwd, t_envs **env_list)
{
	char *old_path;
	char *new_path;
	char *pwd_key_old;
	char *pwd_key_new;

	old_path = ft_strdup(old_pwd);
	if (!old_path)
		return (1);
	new_path = ft_strdup(pwd);
	if (!new_path)
		return (free(old_path), 1);
	pwd_key_old = ft_strdup("OLDPWD");
	if (!pwd_key_old)
		return (free(old_path), free(new_path), 1);
	pwd_key_new = ft_strdup("PWD");
	if (!pwd_key_new)
		return (free(old_path), free(new_path), free(pwd_key_old), 1);
	if (update_or_add(env_list, old_path, pwd_key_old))
		return (1);
	if (update_or_add(env_list, new_path, pwd_key_new))
		return (1);
	return (0);
}

int home_dir(t_cd *cd, t_envs **env_list)
{
	t_envs *tmp;

	tmp = *env_list;
	while(tmp)
	{
		if (ft_strncmp(tmp->key, "HOME", 5) == 0)
		{
			cd->target_path = tmp->value;
			return (0);
		}
		tmp = tmp->next;
	}
	return (1);
}

int old_dir(t_cd *cd, t_envs **env_list)
{
	t_envs *tmp;

	tmp = *env_list;
	while(tmp)
	{
		if (ft_strncmp(tmp->key, "OLDPWD", 7) == 0)
		{
			cd->target_path = tmp->value;
			return (0);
		}
		tmp = tmp->next;
	}
	return (1);
}
