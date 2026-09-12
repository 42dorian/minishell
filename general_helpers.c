/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabdulla <dabdulla@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 22:57:48 by dabdulla          #+#    #+#             */
/*   Updated: 2026/09/12 14:47:03 by dabdulla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_error(char *msg, char *cmd, char *arg, int fd)
{
	int len;
	char *buffer;

	len = ft_strlen("minishell: ") + ft_strlen(cmd) + ft_strlen(msg) + 3;
	if (arg)
		len += ft_strlen(arg) + ft_strlen(": ");
	buffer = ft_calloc(len + 1, sizeof(char));
	if (!buffer)
		return ;
	ft_strlcat(buffer, "minishell: ", len + 1);
	if (cmd)
	{
		ft_strlcat(buffer, cmd, len + 1);
		ft_strlcat(buffer, ": ", len + 1);
	}
	if (arg)
	{
		ft_strlcat(buffer, arg, len + 1);
		ft_strlcat(buffer, ": ", len + 1);
	}
	ft_strlcat(buffer, msg, len + 1);
	ft_strlcat(buffer, "\n", len + 1);
	write(fd, buffer, ft_strlen(buffer));
	free(buffer);
}

void free_all_and_exit(t_shell *shell, int status)
{
	t_cmds *tmp_cmd;
	t_envs *tmp_env;

	tmp_env = shell->env_list;
	tmp_cmd = shell->cmds;
	if (shell->envp)
	{
		free_split(shell->envp);
		shell->envp = NULL;
	}
	if (shell->saved_stdin > 0)
		close(shell->saved_stdin);
	if (shell->saved_stdout > 1)
		close(shell->saved_stdout);
	free_cmds(&shell->cmds);
	ft_lstclear(&tmp_env, free);
	exit(status);
}
