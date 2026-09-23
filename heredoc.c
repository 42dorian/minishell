/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 22:59:23 by dabdulla          #+#    #+#             */
/*   Updated: 2026/09/21 13:13:54 by bguthy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	fill_quoted_heredoc(int write_fd, const char *eof);
static void	fill_unqoted_heredoc(int write_fd, const char *eof, t_envs *env);

static void	fill_quoted_heredoc(int write_fd, const char *eof)
{
	char	*line;

	line = NULL;
	while (1)
	{
		if (isatty(STDIN_FILENO))
			rl_event_hook = event_hook;
		line = readline("> ");
		rl_event_hook = event_hook_main;
		if (g_signal == 42)
			break ;
		if (!line)
		{
			print_heredoc_warning(eof);
			break ;
		}
		if (ft_strncmp(line, eof, ft_strlen(eof) + 1) == 0)
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, write_fd);
		free(line);
	}
}

static void	fill_unqoted_heredoc(int write_fd, const char *eof, t_envs *env)
{
	char	*line;
	char	*line_expanded;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			rl_event_hook = event_hook;
		line = readline("> ");
		rl_event_hook = event_hook_main;
		if (g_signal == 42)
			break ;
		if (!line)
			return (print_heredoc_warning(eof));
		if (ft_strncmp(line, eof, ft_strlen(eof) + 1) == 0)
		{
			free(line);
			break ;
		}
		line_expanded = expanded_line(line, env);
		ft_putendl_fd(line_expanded, write_fd);
		free(line);
		free(line_expanded);
	}
}

int	handle_heredoc(t_cmds *curr, t_token *token, int *i, t_shell *shell)
{
	int	fd[2];

	if (pipe(fd))
		return (print_error(strerror(errno), "pipe", NULL, STDERR_FILENO), 1);
	if (token[*i + 1].quoted)
		fill_quoted_heredoc(fd[1], token[*i + 1].value);
	else
		fill_unqoted_heredoc(fd[1], token[*i + 1].value, shell->env_list);
	close(fd[1]);
	init_interactive_signals();
	if (g_signal == 42)
	{
		g_signal = 0;
		return (close(fd[0]), 130);
	}
	if (curr->fd_in != 0)
		close(curr->fd_in);
	curr->fd_in = fd[0];
	return (0);
}
