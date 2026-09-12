/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabdulla <dabdulla@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 09:57:52 by dabdulla          #+#    #+#             */
/*   Updated: 2026/09/12 12:43:25 by dabdulla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static int	fork_pipe(t_cmds *cmds, int *fd, int *stored_input, char **envp);
static int	fork_pipe(t_cmds *cmds, int *fd, int *stored_input, t_shell *shell);

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
	pause_interactive_signals();
	while (curr_cmd)
	{
		if (!fork_pipe(curr_cmd, fd, &stored_input, shell))
			break;
		clean_parent(curr_cmd, fd, &stored_input);
		curr_cmd = curr_cmd->next;
	}
	wait_pids(shell->cmds, &shell->status);
	init_interactive_signals();
	free_split(shell->envp);
	shell->envp = NULL;
	return (shell->status);
}

int	execute_single_cmd(t_shell *shell)
{
	int	saved_stdin;
	int	saved_stdout;

	if (!shell->cmds->cmd)
	{
		shell->status = 0;
		return (1);
	}
	if (shell->cmds->fd_in == -1 || shell->cmds->fd_out == -1)
	{
		shell->status = 1;
		return (1);
	}
	if (is_built_in(shell->cmds->cmd[0]))
	{
		saved_stdin = dup(STDIN_FILENO);
		saved_stdout = dup(STDOUT_FILENO);
		if (saved_stdin == -1)
			return (print_error(strerror(errno), "dup", NULL, 2), 1);
		else if (saved_stdout == -1)
			return (close(saved_stdin), print_error(strerror(errno), "dup", NULL, 2), 1);
		if (change_io(shell->cmds))
		{
			restore_io(saved_stdin, saved_stdout);
			return (1);
		}
		shell->status = run_built_in(shell->cmds, shell->env_list, shell);
		restore_io(saved_stdin, saved_stdout);
		return (1);
	}
	if (!run_cmd(shell->cmds, shell->envp, &shell->status, shell))
		return (0);
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
		return (free(path), print_error(strerror(errno), cmd->cmd[0], NULL, 2), 0);
	if (cmd->pid == 0)
	{
		init_execution_signals();
		if (cmd->fd_in != 0)
		{
			safe_dup2(cmd->fd_in, STDIN_FILENO);
			close(cmd->fd_in);
		}
		if (cmd->fd_out != 1)
		{
			safe_dup2(cmd->fd_out, STDOUT_FILENO);
			close(cmd->fd_out);
		}
		execve(path, cmd->cmd, envp);
		print_error(strerror(errno), cmd->cmd[0], NULL, 2);
		free_all_and_exit(shell, 1);
	}
	pause_interactive_signals();
	wait_single_pid(cmd->pid, status, 1);
	init_interactive_signals();
	return (free(path), 1);
}

void wait_single_pid(pid_t pid, int *status, int last_pid)
{
	int raw_status;

	if (pid > 0)
	{
		if (waitpid(pid, &raw_status, 0) == -1)
			return ;
		if (WIFEXITED(raw_status))
			*status = WEXITSTATUS(raw_status);
		else if(WIFSIGNALED(raw_status))
		{
			if (WTERMSIG(raw_status) == SIGQUIT && last_pid)
				write(STDERR_FILENO, "Quit: (core dumped)\n", 20);
			else if(WTERMSIG(raw_status) == SIGINT && last_pid)
				write(STDERR_FILENO, "\n", 1);
			*status = 128 + WTERMSIG(raw_status);
		}
	}
}

void	run_child(t_cmds *cmds, int *fd, int stored_input, t_shell *shell)
{
	char	*path;
	int exit_status;

	exit_status = 0;
	if (!cmds->cmd || !cmds->cmd[0])
		free_all_and_exit(shell, 0);
	if (cmds->fd_in == -1 || cmds->fd_out == -1)
		free_all_and_exit(shell, 1);
	child_redirections(cmds, fd, stored_input);
	close_inherited_fds(cmds);
	if (is_built_in(cmds->cmd[0]))
	{
		exit_status = run_built_in(cmds, shell->env_list, shell);
		free_all_and_exit(shell, exit_status);
	}
	path = handling_path(cmds->cmd[0], shell->envp[find_path(shell->envp)], &exit_status);
	if (!path)
		free_all_and_exit(shell, exit_status);
	execve(path, cmds->cmd, shell->envp);
	print_error(strerror(errno), cmds->cmd[0], NULL, 2);
	free_all_and_exit(shell, 1);
}

static int	fork_pipe(t_cmds *cmds, int *fd, int *stored_input, t_shell *shell)
{
	if (cmds->next)
	{
		if (pipe(fd) == -1)
			return (0);
	}
	cmds->pid = fork();
	if (cmds->pid == -1)
	{
		if (cmds->next)
		{
			close(fd[0]);
			close(fd[1]);
		}
		print_error(strerror(errno), "fork", NULL, STDERR_FILENO);
		return (0);
	}
	if (cmds->pid == 0)
	{
		init_execution_signals();
		run_child(cmds, fd, *stored_input, shell);
	}
	return (1);
}
