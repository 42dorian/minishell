/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabdulla <dabdulla@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 09:57:52 by dabdulla          #+#    #+#             */
/*   Updated: 2026/09/12 14:38:10 by dabdulla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	execute_single_built_in(t_shell *shell);

int open_redirections(t_cmds *cmds)
{
	t_redirs *curr;

	curr = cmds->redirs;
	while (curr)
	{
		if (curr->is_out == 0)
		{
			if (cmds->fd_in != 0)
				close(cmds->fd_in);
			cmds->fd_in = open(curr->filename, curr->flags);
			if (cmds->fd_in == -1)
				return (print_error(strerror(errno), curr->filename, NULL, STDERR_FILENO), 1);
		}
		else
		{
			if (cmds->fd_out != 1)
				close(cmds->fd_out);
			cmds->fd_out = open(curr->filename, curr->flags, 0644);
			if (cmds->fd_out == -1)
				return (print_error(strerror(errno), curr->filename, NULL, STDERR_FILENO), 1);
		}
		curr = curr->next;
	}
	return (0);
}

int	execute_cmds(t_shell *shell)
{
	t_cmds	*curr_cmd;
	int		fd[2];
	int		stored_input;

	if (!shell->cmds)
		return (0);
	shell->envp = create_envp(shell->env_list);
	if (!shell->envp)
		return (0);
	stored_input = -1;
	ft_bzero(fd, sizeof(fd));
	curr_cmd = shell->cmds;
	if (!curr_cmd->next)
	{
		execute_single_cmd(shell);
		clean_parent(curr_cmd, fd, &stored_input);
		free_split(shell->envp);
		shell->envp = NULL;
		return (shell->status);
	}
	else
		execute_pipeline(shell, curr_cmd, fd, &stored_input);
	free_split(shell->envp);
	shell->envp = NULL;
	return (shell->status);
}

int	execute_single_cmd(t_shell *shell)
{
	if (!shell->cmds->cmd)
	{
		shell->status = 0;
		return (1);
	}
	if (open_redirections(shell->cmds))
	{
		shell->status = 1;
		return (1);
	}
	if (shell->cmds->fd_in == -1 || shell->cmds->fd_out == -1)
	{
		shell->status = 1;
		return (1);
	}
	if (is_built_in(shell->cmds->cmd[0]))
		return (execute_single_built_in(shell));
	if (!run_cmd(shell->cmds, shell->envp, &shell->status, shell))
		return (0);
	return (1);
}

int	execute_single_built_in(t_shell *shell)
{
	shell->saved_stdin = dup(STDIN_FILENO);
	shell->saved_stdout = dup(STDOUT_FILENO);
	if (shell->saved_stdin == -1)
		return (print_error(strerror(errno), "dup", NULL, 2), 1);
	else if (shell->saved_stdout == -1)
		return (close(shell->saved_stdin), print_error(strerror(errno), "dup",
				NULL, 2), 1);
	if (change_io(shell->cmds))
	{
		restore_io(shell->saved_stdin, shell->saved_stdout);
		return (1);
	}
	shell->status = run_built_in(shell->cmds, shell->env_list, shell);
	restore_io(shell->saved_stdin, shell->saved_stdout);
	shell->saved_stdin = -1;
	shell->saved_stdout = -1;
	return (1);
}

int	run_cmd(t_cmds *cmd, char **envp, int *status, t_shell *shell)
{
	char	*path;

	if (!cmd->cmd || !cmd->cmd[0])
		return (0);
	path = handling_path(cmd->cmd[0], envp[find_path(envp)], status);
	if (!path)
		return (0);
	cmd->pid = fork();
	if (cmd->pid == -1)
		return (free(path), print_error(strerror(errno), cmd->cmd[0], NULL, 2),
			0);
	if (cmd->pid == 0)
		exec_child_process(cmd, path, envp, shell);
	pause_interactive_signals();
	wait_single_pid(cmd->pid, status, 1);
	init_interactive_signals();
	return (free(path), 1);
}

void	run_child(t_cmds *cmds, int *fd, int stored_input, t_shell *shell)
{
	char	*path;
	int		exit_status;

	exit_status = 0;
	if (open_redirections(cmds))
		free_all_and_exit(shell, 1);
	check_child_fds(cmds, fd, stored_input, shell);
	child_redirections(cmds, fd, stored_input);
	close_inherited_fds(cmds);
	if (cmds->fd_in == -1 || cmds->fd_out == -1)
		free_all_and_exit(shell, 1);
	if (!cmds->cmd || !cmds->cmd[0])
		free_all_and_exit(shell, shell->status);
	if (is_built_in(cmds->cmd[0]))
	{
		exit_status = run_built_in(cmds, shell->env_list, shell);
		free_all_and_exit(shell, exit_status);
	}
	path = handling_path(cmds->cmd[0], shell->envp[find_path(shell->envp)],
			&exit_status);
	if (!path)
		free_all_and_exit(shell, exit_status);
	signal(SIGPIPE, SIG_DFL);
	execve(path, cmds->cmd, shell->envp);
	free(path);
	print_error(strerror(errno), cmds->cmd[0], NULL, 2);
	free_all_and_exit(shell, 1);
}
