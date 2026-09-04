#include "minishell.h"

static int remove_env(char *cmd, t_envs *tmp, t_envs *env_list);

int unset(t_cmds *cmd, t_envs *env_list)
{
	t_envs *tmp;
	int i;
	if (!cmd->cmd[1])
		return (0);
	tmp = env_list;
	i = 0;
	while (cmd->cmd[i])
	{
		remove_env(cmd->cmd[i], tmp, env_list);
		i++;
	}
	return (0);
}

int remove_env(char *cmd, t_envs *tmp, t_envs *env_list)
{
	while (tmp)
	{
		if (!tmp->next)
			break;
		if (ft_strncmp(cmd, tmp->next->key, ft_strlen(cmd)) == 0)
		{
			free(tmp->next->key);
			free(tmp->next->value);
			tmp->next = tmp->next->next;
			env_list = tmp;
			return (0);
		}
		tmp = tmp->next;
	}
	return (0);
}
