/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabdulla <dabdulla@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 10:32:32 by dabdulla          #+#    #+#             */
/*   Updated: 2026/09/07 23:03:08 by dabdulla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	fill_heredoc(int write_fd, char *eof, t_envs *env);

int	handle_in(t_cmds *curr, t_token *tokens, int *i)
{
	if (curr->fd_in != 0)
		close(curr->fd_in);
	curr->fd_in = open(tokens[*i + 1].value, O_RDONLY);
	if (curr->fd_in == -1)
		print_error(strerror(errno), tokens[*i + 1].value, NULL, 2);
	return (1);
}

int	handle_out(t_cmds *curr, t_token *token, int *i)
{
	if (curr->fd_out != 1)
		close(curr->fd_out);
	if (token[*i].type == token_redirect_out)
	{
		curr->fd_out = open(token[*i + 1].value, O_WRONLY | O_CREAT | O_TRUNC,
				0644);
		if (curr->fd_out == -1)
			print_error(strerror(errno), token[*i + 1].value, NULL, 2);
	}
	if (token[*i].type == token_append)
	{
		curr->fd_out = open(token[*i + 1].value, O_WRONLY | O_CREAT | O_APPEND,
				0644);
		if (curr->fd_out == -1)
			print_error(strerror(errno), token[*i + 1].value, NULL, 2);
	}
	return (1);
}

int	handle_pipe(t_cmds **head, t_cmds **curr)
{
	t_cmds	*next;

	next = NULL;
	next = new_cmd();
	if (!next)
		return (0);
	*head = add_cmd(*head, next);
	if (!*head)
		return (0);
	*curr = next;
	return (1);
}

// char *expanded_line(char *line, t_envs *env)
// {
// 	t_token token;
// 	t_token array[1];
// 	int len;
// 	char *expanded;

// 	token.value = line;
// 	token.type = token_word;

// 	array[0].type = 0;
// 	len = get_full_len_of_expandable(token, env, &token);
// 	expanded = get_full_expandable_word(token, env, len, 0);
// 	return (expanded);
// }

static void	fill_heredoc(int write_fd, char *eof, t_envs *env)
{
	char	*line;
	char 	*line_expanded;
	int line_count;
	t_token *tokens;

	tokens = NULL;
	line = NULL;
	line_expanded = NULL;
	line_count = 0;
	while (1)
	{
		line_count++;
		line = readline("> ");
		if (!line)
		{
			ft_putstr_fd("minishell: warning: ", STDOUT_FILENO);
			ft_putstr_fd("here-document delimited by end-of-file (wanted '", STDOUT_FILENO);
			ft_putstr_fd(eof, STDOUT_FILENO);
			ft_putstr_fd("')\n", STDOUT_FILENO);
		}
		if ((ft_strlen(line) == ft_strlen(eof) && ft_strncmp(line, eof,ft_strlen(eof)) == 0))
		{
			free(line);
			break ;
		}
		// line_expanded = expanded_line(line, env);
		ft_putstr_fd(line, write_fd);
		ft_putchar_fd('\n', write_fd);
		free(line);
		// free(line_expanded);
	}
}

int	handle_heredoc(t_cmds *curr, t_token *token, int *i, t_envs *env)
{
	int	fd[2];
	pid_t pid;
	int status;

	status = 0;
	if (pipe(fd) == -1)
		return (print_error(strerror(errno), "maybe *token[*i]", NULL, STDERR_FILENO), 0);
	pause_interactive_signals();
	pid = fork();
	if (pid == 0)
	{
		init_heredoc_signals();
		fill_heredoc(fd[1], token[*i + 1].value, env);
		exit(0);
	}
	close(fd[1]);
	wait_single_pid(pid, &status, 1);
	init_interactive_signals();
	if (status == 130)
		return (0);
	if (curr->fd_in != 0)
		close(curr->fd_in);
	curr->fd_in = fd[0];
	return (1);
}
