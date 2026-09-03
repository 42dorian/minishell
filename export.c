#include "minishell.h"

char *add_key(char *key);
char *add_value(char *value);
int key_exists(char *key, t_envs **env_list);
int add_new_list(t_envs *new, char **key_value, char *cmd, t_envs *env_list);


int export_bi(t_cmds *cmd, t_envs *env_list)
{
	t_envs *new;
	char **key_value;
	int i;

	if (cmd->cmd[1] == NULL)
		return (1);
	i = 1;
	while (cmd->cmd[i])
	{
		if (add_new_list(new, key_value, cmd->cmd[i], env_list) == 1)
			return (1);
		i++;
	}
	return (0);
}


int add_new_list(t_envs *new, char **key_value, char *cmd, t_envs *env_list)
{
	key_value = ft_split(cmd, '=');
	if (!key_value)
		return (1);
	if (key_exists(key_value[0], &env_list))
	{
		free(env_list->value);
		env_list->value = add_value(key_value[1]);
		if (!env_list->value)
			return (1);
		return (0);
	}
	new = ft_calloc(1, sizeof(t_envs));
	if (!new)
		return (1);
	new->key = add_key(key_value[0]);
	if (!new->key)
		return (free(new), 1);
	new->value = add_value(key_value[1]);
	if (!new->value)
		return (1);
	ft_lstadd_back(&env_list, new);
	free_split(key_value);
	return (0);
}

char *add_key(char *key)
{
	char *tmp;

	tmp = ft_strdup(key);
	if (!tmp)
		return (NULL);
	return (tmp);
}

char *add_value(char *value)
{
	char *tmp;

	tmp = ft_strdup(value);
	if (!tmp)
		return (NULL);
	return (tmp);
}

int key_exists(char *key, t_envs **env_list)
{
	t_envs *tmp;

	tmp = *env_list;
	while(tmp)
	{
		if (ft_strncmp(key, tmp->key, ft_strlen(key)) == 0)
		{
			*env_list = tmp;
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}
