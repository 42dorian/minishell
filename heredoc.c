/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabdulla <dabdulla@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 22:59:23 by dabdulla          #+#    #+#             */
/*   Updated: 2026/09/12 12:50:25 by dabdulla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	fill_quoted_heredoc(int write_fd, char *eof, t_envs *env);
static char	*expanded_line(char *line, t_envs *env);
static int	contains_quotes(char *str);
static void	print_heredoc_warning(char *eof);

static void	print_heredoc_warning(char *eof)
{
	ft_putstr_fd("minishell: warning: ", STDOUT_FILENO);
	ft_putstr_fd("here-document delimited by end-of-file (wanted '",
		STDOUT_FILENO);
	ft_putstr_fd(eof, STDOUT_FILENO);
	ft_putstr_fd("')\n", STDOUT_FILENO);
}

static char	*expanded_line(char *line, t_envs *env)
{
	t_token	token;
	t_token	array[1];
	int		len;
	char	*expanded;
	int exit_code;

	*exit_code = 0;
	token.value = line;
	token.type = token_word;
	array[0].type = 0;
	len = get_full_len_of_expandable(token, env, &token, &exit_code);
	expanded = get_full_expandable_word(token, env, len, 0);
	return (expanded);
}

static void	fill_quoted_heredoc(int write_fd, char *eof, t_envs *env)
{
	char	*line;

	line = NULL;
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			print_heredoc_warning(eof);
			return ;
		}
		if (ft_strncmp(line, eof, ft_strlen(eof) + 1) == 0)
		{
			free(line);
			return ;
		}
		ft_putstr_fd(line, write_fd);
		ft_putchar_fd('\n', write_fd);
		free(line);
	}
}

static void	fill_unqoted_heredoc(int write_fd, char *eof, t_envs *env)
{
	char	*line;
	char	*line_expanded;

	line = NULL;
	line_expanded = NULL;
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			print_heredoc_warning(eof);
			return ;
		}
		if (ft_strncmp(line, eof, ft_strlen(eof) + 1) == 0)
		{
			free(line);
			break ;
		}
		line_expanded = expanded_line(line, env);
		ft_putstr_fd(line_expanded, write_fd);
		ft_putchar_fd('\n', write_fd);
		free(line_expanded);
		free(line);
	}
}

int	handle_heredoc(t_cmds *curr, t_token *token, int *i, t_envs *env)
{
	int		fd[2];
	pid_t	pid;
	int		status;

	status = 0;
	if (pipe(fd) == -1)
		return (print_error(strerror(errno), "pipe", NULL, STDERR_FILENO), 1);
	pause_interactive_signals();
	pid = fork();
	if (pid == -1)
	{
		close(fd[0]);
		close(fd[1]);
		return (print_error(strerror(errno), "fork", NULL, STDERR_FILENO), 1);
	}
	if (pid == 0)
	{
		close(fd[0]);
		init_heredoc_signals();
		if (token[*i + 1].quoted)
			fill_quoted_heredoc(fd[1], token[*i + 1].value, env);
		else
			fill_unqoted_heredoc(fd[1], token[*i + 1].value, env);
		free_tokens(token);
		free_cmd(curr);
		ft_lstclear(&env, free);
		close(fd[1]);
		exit(0);
	}
	close(fd[1]);
	wait_single_pid(pid, &status, 1);
	init_interactive_signals();
	if (status != 0)
		return (close(fd[0]), status);
	if (curr->fd_in != 0)
		close(curr->fd_in);
	curr->fd_in = fd[0];
	return (status);
}
