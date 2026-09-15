/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_executor.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabdulla <dabdulla@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 11:15:38 by dabdulla          #+#    #+#             */
/*   Updated: 2026/09/12 12:01:54 by dabdulla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_built_in(char *cmd)
{
	int		i;
	char	**bi;

	if (!cmd)
		return (0);
	i = 0;
	bi = (char *[]){"echo", "cd", "pwd", "export", "unset", "env", "exit",
		NULL};
	while (bi[i])
	{
		if (ft_strlen(bi[i]) == ft_strlen(cmd) && (ft_strncmp(bi[i], cmd,
					ft_strlen(bi[i])) == 0))
			return (1);
		i++;
	}
	return (0);
}

int	run_built_in(t_cmds *cmd, t_envs *env_list, t_shell *shell)
{
	int	cmd_len;

	cmd_len = ft_strlen(cmd->cmd[0]);
	if (cmd_len == 3 && ft_strncmp(cmd->cmd[0], "pwd", 3) == 0)
		return (pwd());
	if (cmd_len == 4 && ft_strncmp(cmd->cmd[0], "echo", 4) == 0)
		return (echo(cmd));
	if (cmd_len == 3 && ft_strncmp(cmd->cmd[0], "env", 3) == 0)
		return (env(env_list));
	if (cmd_len == 6 && ft_strncmp(cmd->cmd[0], "export", 6) == 0)
			return (export_bi(cmd, &shell->env_list));
	if (cmd_len == 5 && ft_strncmp(cmd->cmd[0], "unset", 5) == 0)
		return (unset(cmd, &shell->env_list));
	if (cmd_len == 4 && ft_strncmp(cmd->cmd[0], "exit", 4) == 0)
		return (exit_bi(cmd, shell));
	if (cmd_len == 2 && ft_strncmp(cmd->cmd[0], "cd", 2) == 0)
		return (cd_bi(cmd, &shell->env_list));
	return (0);
}

int	change_io(t_cmds *cmds)
{
	int	return_value;

	return_value = 0;
	if (cmds->fd_in != 0)
	{
		if (dup2(cmds->fd_in, STDIN_FILENO) == -1)
		{
			return_value = 1;
			print_error(strerror(errno), "dup2", NULL, STDERR_FILENO);
		}
		else
		{
			close(cmds->fd_in);
			cmds->fd_in = 0;
		}
	}
	if (cmds->fd_out != 1)
	{
		if (dup2(cmds->fd_out, STDOUT_FILENO) == -1)
		{
			return_value = 1;
			print_error(strerror(errno), "dup2", NULL, STDERR_FILENO);
		}
		else
		{
			close(cmds->fd_out);
			cmds->fd_out = 1;
		}
	}
	return (return_value);
}
