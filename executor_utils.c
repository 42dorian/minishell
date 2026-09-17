/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabdulla <dabdulla@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 16:45:57 by dabdulla          #+#    #+#             */
/*   Updated: 2026/09/17 16:45:59 by dabdulla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_pipeline(t_shell *shell, t_cmds *curr_cmd, int *fd, int *st_in)
{
	pause_interactive_signals();
	while (curr_cmd)
	{
		if (!fork_pipe(curr_cmd, fd, st_in, shell))
		{
			if (*st_in != -1)
				close(*st_in);
			*st_in = -1;
			break ;
		}
		clean_parent(curr_cmd, fd, st_in);
		curr_cmd = curr_cmd->next;
	}
	if (*st_in != -1)
	{
		close(*st_in);
		*st_in = -1;
	}
	wait_pids(shell->cmds, &shell->status);
	init_interactive_signals();
}

void	exec_child_process(t_cmds *cmd, char *path, char **envp, t_shell *shell)
{
	init_execution_signals();
	if (cmd->fd_in != 0)
	{
		safe_dup2(cmd, cmd->fd_in, STDIN_FILENO);
		close(cmd->fd_in);
	}
	if (cmd->fd_out != 1)
	{
		safe_dup2(cmd, cmd->fd_out, STDOUT_FILENO);
		close(cmd->fd_out);
	}
	execve(path, cmd->cmd, envp);
	print_error(strerror(errno), cmd->cmd[0], NULL, 2);
	free_all_and_exit(shell, 1);
}

void	check_child_fds(t_cmds *cmds, int *fd, int stored_input, t_shell *shell)
{
	int	exit_code;

	exit_code = 0;
	if (!cmds->cmd || !cmds->cmd[0])
		exit_code = 0;
	else if (cmds->fd_in == -1 || cmds->fd_out == -1)
		exit_code = 1;
	else
		return ;
	if (stored_input != -1)
		close(stored_input);
	if (cmds->next)
	{
		close(fd[0]);
		close(fd[1]);
	}
	close_inherited_fds(cmds);
	free_all_and_exit(shell, shell->status);
}

int	fork_pipe(t_cmds *cmds, int *fd, int *stored_input, t_shell *shell)
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

void	wait_single_pid(pid_t pid, int *status, int last_pid)
{
	int	raw_status;
	int	code;

	code = 0;
	if (pid <= 0 || waitpid(pid, &raw_status, 0) == -1)
		return ;
	if (WIFEXITED(raw_status))
		code = WEXITSTATUS(raw_status);
	else if (WIFSIGNALED(raw_status))
	{
		if (WTERMSIG(raw_status) == SIGQUIT && last_pid)
			write(STDERR_FILENO, "Quit: (core dumped)\n", 20);
		else if (WTERMSIG(raw_status) == SIGINT && last_pid)
			write(STDERR_FILENO, "\n", 1);
		code = 128 + WTERMSIG(raw_status);
	}
	else
		code = 0;
	if (last_pid)
		*status = code;
}
