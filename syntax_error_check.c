/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error_check.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 16:08:24 by bguhty            #+#    #+#             */
/*   Updated: 2026/09/17 15:02:15 by bguthy           ###   ########.fr       */
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


int	pipe_check_linked_list(t_new_token *tokens, int i)
{
	t_new_token *next;

	next = tokens->next;
	if (i == 0 && tokens->type == token_pipe)
		return (syntax_error_message_display(tokens->value));
	// else if (i > 0 && tokens->type == token_pipe && tokens->type != token_word)
	// 	return (syntax_error_message_display(tokens->value));
	else if (tokens->type == token_pipe && (next->type == token_pipe
			|| next->value == NULL))
		return (syntax_error_message_display(next->value));
	else if (tokens->type == token_pipe && next->type != token_word)
		return (syntax_error_message_display(next->value));
	return (0);
}


int	redir_check_linked_list(t_new_token *tokens, int i)
{
	t_new_token *next;

	next = tokens->next;
	if (tokens->type == token_append && next->type != token_word)
		return (syntax_error_message_display(next->value));
	else if (tokens->type == token_redirect_in && next->type != token_word)
		return (syntax_error_message_display(next->value));
	else if (tokens->type == token_redirect_out && next->type != token_word)
		return (syntax_error_message_display(next->value));
	else if (tokens->type == token_heredoc && next->type != token_word)
		return (syntax_error_message_display(next->value));
	else
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


int	special_character_syntax_checker_linked_list(t_new_token *tokens,int i)
{
	if (pipe_check_linked_list(tokens, i) || redir_check_linked_list(tokens, i))
		return (1);
	else
		return (0);
}

void	syntax_check_linked_list(t_new_token *tokens, int *status)
{
	int	i;

	i = 0;
	if (preliminary_check_linked_list(tokens))
	{
		*status = 2;
		return ;
	}
	while (tokens)
	{
		if (special_character_syntax_checker_linked_list(tokens, i))
		{
			*status = 2;
			break ;
		}
		tokens = tokens->next;
		i++;
	}
}

void	syntax_check(t_token *tokens, int *status)
{
	int	i;

	i = 0;
	if (preliminary_check(tokens))
	{
		*status = 2;
		return ;
	}
	while (tokens[i].value)
	{
		if (special_character_syntax_checker(tokens, i))
		{
			*status = 2;
			break ;
		}
		i++;
	}
}
