/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabdulla <dabdulla@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/05 17:45:55 by dabdulla          #+#    #+#             */
/*   Updated: 2026/09/12 12:21:38 by dabdulla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_envs	*key_exists(char *key, t_envs *env_list);
static int		add_new_list(char *cmd, t_envs **env_list);
static int		print_envs(t_envs *env_list);

int	export_bi(t_cmds *cmd, t_envs **env_list)
{
	int	i;
	int	return_val;
	int	exit_code;

	if (cmd->cmd[1] == NULL)
		return (print_envs(*env_list));
	i = 1;
	exit_code = 0;
	while (cmd->cmd[i])
	{
		return_val = add_new_list(cmd->cmd[i], env_list);
		if (return_val == 1)
			return (1);
		else if (return_val == 2)
		{
			ft_putstr_fd("minishell: export: `", STDERR_FILENO);
			ft_putstr_fd(cmd->cmd[i], STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			exit_code = 1;
		}
		i++;
	}
	return (exit_code);
}

static int	add_new_list(char *cmd, t_envs **env_list)
{
	char	*equal_pos;
	char	*key;
	char	*value;

	equal_pos = ft_strchr(cmd, '=');
	if (!equal_pos)
	{
		key = ft_strdup(cmd);
		if (!key)
			return (1);
		value = NULL;
	}
	else
	{
		key = ft_substr(cmd, 0, equal_pos - cmd);
		if (!key)
			return (1);
		value = ft_strdup(equal_pos + 1);
		if (!value)
			return (free(key), 1);
	}
	if (valid_identifier(key))
		return (free(value), free(key), 2);
	return (update_or_add(env_list, value, key));
}

int	update_or_add(t_envs **env_list, char *value, char *key)
{
	t_envs	*head;
	t_envs	*new;

	head = key_exists(key, *env_list);
	if (head)
	{
		if (value)
		{
			free(head->value);
			head->value = value;
		}
		free(key);
	}
	else
	{
		new = ft_calloc(1, sizeof(t_envs));
		if (!new)
		{
			free(key);
			if (value)
				free(value);
			return (1);
		}
		new->key = key;
		new->value = value;
		ft_lstadd_back(env_list, new);
	}
	return (0);
}

static t_envs	*key_exists(char *key, t_envs *env_list)
{
	while (env_list)
	{
		if (ft_strncmp(key, env_list->key, ft_strlen(key) + 1) == 0)
			return (env_list);
		env_list = env_list->next;
	}
	return (NULL);
}

static int	print_envs(t_envs *env_list)
{
	t_envs	*tmp;

	tmp = env_list;
	while (tmp)
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(tmp->key, STDOUT_FILENO);
		if (tmp->value != NULL)
		{
			ft_putstr_fd("=\"", STDOUT_FILENO);
			ft_putstr_fd(tmp->value, STDOUT_FILENO);
			ft_putchar_fd('"', STDOUT_FILENO);
		}
		ft_putstr_fd("\n", STDOUT_FILENO);
		tmp = tmp->next;
	}
	return (0);
}
