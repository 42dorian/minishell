/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error_check.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 16:08:24 by bguhty            #+#    #+#             */
/*   Updated: 2026/09/17 18:13:23 by bguthy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	syntax_error_message_display(const char *token_value)
{
	if (!token_value)
	{
		write(STDERR_FILENO,
			"minishell: syntax error near unexpected token `newline'\n", 56);
	}
	else
	{
		write(STDERR_FILENO, "minishell: syntax error near unexpected token `",
			47);
		write(STDERR_FILENO, token_value, ft_strlen(token_value));
		write(STDERR_FILENO, "'\n", 2);
	}
	return (1);
}

int	pipe_check(t_token *tokens, int i)
{
	if (tokens[0].type == token_pipe)
		return (syntax_error_message_display(tokens[i].value));
	else if (i > 0 && tokens[i].type == token_pipe && tokens[i
			- 1].type != token_word)
		return (syntax_error_message_display(tokens[i].value));
	else if (tokens[i].type == token_pipe && (tokens[i + 1].type == token_pipe
			|| tokens[i + 1].value == NULL))
		return (syntax_error_message_display(tokens[i + 1].value));
	else if (tokens[i].type == token_pipe && tokens[i - 1].type != token_word)
		return (syntax_error_message_display(tokens[i + 1].value));
	return (0);
}

int	redir_check(t_token *tokens, int i)
{
	if (tokens[i].type == token_append && tokens[i + 1].type != token_word)
		return (syntax_error_message_display(tokens[i + 1].value));
	else if (tokens[i].type == token_redirect_in && tokens[i
			+ 1].type != token_word)
		return (syntax_error_message_display(tokens[i + 1].value));
	else if (tokens[i].type == token_redirect_out && tokens[i
			+ 1].type != token_word)
		return (syntax_error_message_display(tokens[i + 1].value));
	else if (tokens[i].type == token_heredoc && tokens[i
			+ 1].type != token_word)
		return (syntax_error_message_display(tokens[i + 1].value));
	else
		return (0);
}

int	special_character_syntax_checker(t_token *tokens, int i)
{
	if (pipe_check(tokens, i) || redir_check(tokens, i))
		return (1);
	else
		return (0);
}

int	syntax_check(t_token *tokens, int *status)
{
	int	i;

	i = 0;
	if (preliminary_check(tokens))
	{
		*status = 2;
		return (1);
	}
	while (tokens[i].value)
	{
		if (special_character_syntax_checker(tokens, i))
		{
			*status = 2;
			return (1);
		}
		i++;
	}
	return (0);
}
