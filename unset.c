/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabdulla <dabdulla@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/05 17:45:45 by dabdulla          #+#    #+#             */
/*   Updated: 2026/09/05 20:06:39 by dabdulla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	remove_env(char *cmd, t_envs **env_list);

int	unset(t_cmds *cmd, t_envs **env_list)
{
	int	i;
	int	exit_code;

	exit_code = 0;
	if (!cmd->cmd[1])
		return (0);
	i = 1;
	while (cmd->cmd[i])
	{
		if (valid_identifier(cmd->cmd[i]))
		{
			// ft_putstr_fd("minishell: unset: '", STDERR_FILENO);
			// ft_putstr_fd(cmd->cmd[i], STDERR_FILENO);
			// ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			exit_code = 0;
		}
		else
			remove_env(cmd->cmd[i], env_list);
		i++;
	}
	return (exit_code);
}

int	remove_env(char *cmd, t_envs **env_list)
{
	t_envs	*tmp;
	t_envs	*prev;

	if (!env_list || !*env_list)
		return (0);
	tmp = *env_list;
	prev = NULL;
	while (tmp)
	{
		if (ft_strncmp(cmd, tmp->key, ft_strlen(cmd) + 1) == 0)
		{
			if (prev == NULL)
				*env_list = tmp->next;
			else
				prev->next = tmp->next;
			free(tmp->key);
			free(tmp->value);
			free(tmp);
			return (0);
		}
		prev = tmp;
		tmp = tmp->next;
	}
	return (0);
}

int	valid_identifier(char *key)
{
	int	i;

	if (!key || key[0] == '\0')
		return (1);
	i = 0;
	if (!ft_isalpha(key[0]) && key[0] != '_')
		return (1);
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (1);
		i++;
	}
	return (0);
}
