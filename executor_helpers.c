/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabdulla <dabdulla@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 10:02:58 by dabdulla          #+#    #+#             */
/*   Updated: 2026/09/12 12:31:09 by dabdulla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	safe_dup2(t_cmds *cmd, int oldfd, int newfd)
{
	if (dup2(oldfd, newfd) == -1)
	{
		print_error(strerror(errno), cmd->cmd[0], NULL, STDERR_FILENO);
		exit(1);
	}
}

void	clean_parent(t_cmds *cmds, int *fd, int *stored_input)
{
	if (*stored_input != -1)
	{
		close(*stored_input);
		*stored_input = -1;
	}
	if (cmds->fd_in != 0 && cmds->fd_in != -1)
	{
		close(cmds->fd_in);
		cmds->fd_in = 0;
	}
	if (cmds->fd_out != 1 && cmds->fd_out != -1)
	{
		close(cmds->fd_out);
		cmds->fd_out = 1;
	}
	if (cmds->next)
	{
		close(fd[1]);
		*stored_input = fd[0];
	}
}

void	wait_pids(t_cmds *cmds, int *status)
{
	int	last_pid;

	last_pid = 0;
	while (cmds)
	{
		if (cmds->next == NULL)
			last_pid = 1;
		if (cmds->pid > 0)
			wait_single_pid(cmds->pid, status, last_pid);
		cmds = cmds->next;
	}
}

void	child_redirections(t_cmds *cmds, int *fd, int stored_input)
{
	if (stored_input != -1)
	{
		safe_dup2(cmds, stored_input, STDIN_FILENO);
		close(stored_input);
	}
	if (cmds->next)
	{
		safe_dup2(cmds, fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
	}
	if (cmds->fd_in > 0)
	{
		safe_dup2(cmds, cmds->fd_in, STDIN_FILENO);
		close(cmds->fd_in);
	}
	if (cmds->fd_out > 1)
	{
		safe_dup2(cmds, cmds->fd_out, STDOUT_FILENO);
		close(cmds->fd_out);
	}
}

void	close_inherited_fds(t_cmds *cmds)
{
	t_cmds	*tmp;

	if (!cmds)
		return ;
	tmp = cmds;
	while (tmp->prev)
		tmp = tmp->prev;
	tmp = tmp->next;
	while (tmp)
	{
		if (tmp->fd_in != 0 && tmp->fd_in != -1)
			close(tmp->fd_in);
		if (tmp->fd_out != 1 && tmp->fd_out != -1)
			close(tmp->fd_out);
		tmp = tmp->next;
	}
}
