/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error_check.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guthybarnakoppany <guthybarnakoppany@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 16:08:24 by bguhty            #+#    #+#             */
/*   Updated: 2026/09/09 12:58:47 by guthybarnak      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int     syntax_error_message_display(const char *token_value)
{
    if (!token_value)
    {
    	ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", STDERR_FILENO);
    }
    else
    {
    	ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
     	ft_putstr_fd((char *)token_value, STDERR_FILENO);
      	write(STDERR_FILENO, "'\n", 2);
    }

    return (1);
}

int pipe_check(t_token *tokens, int i)
{
    if (tokens[0].type == token_pipe)
        return (syntax_error_message_display(tokens[i].value));
    if (tokens[i].type == token_pipe && (tokens[i + 1].type == token_pipe || tokens[i + 1].value == NULL))
        return (syntax_error_message_display(tokens[i + 1].value));
    if (tokens[i].type == token_pipe && tokens[i - 1].type != token_word)
        return (syntax_error_message_display(tokens[i + 1].value));
    return (0);
}

int redir_check(t_token *tokens, int i)
{
    if (tokens[i].type == token_append && tokens[i + 1].type != token_word)
        return (syntax_error_message_display(tokens[i + 1].value));
    else if (tokens[i].type == token_redirect_in && tokens[i + 1].type != token_word)
        return (syntax_error_message_display(tokens[i + 1].value));
    else if (tokens[i].type == token_redirect_out && tokens[i + 1].type != token_word)
        return (syntax_error_message_display(tokens[i + 1].value));
    return (0);
}

int heredoc_check(t_token *tokens, int i)
{
    if (tokens[i].type == token_heredoc)
    {
        if (tokens[i + 1].type != token_word)
            return (syntax_error_message_display(tokens[i + 1].value));
    }
    return (0);
}

int special_character_syntax_checker(t_token *tokens, int i)
{
    if (pipe_check(tokens, i))
        return (1);
    else if (heredoc_check(tokens, i))
        return (1);
    else if (redir_check(tokens, i))
        return (1);
    else
        return (0);
}

int get_list_size(t_token *tokens)
{
    int size;

    size = 0;
    while (tokens[size].type != -1)
        size++;
    return (size);
}

int is_special_character(const char letter1, const char letter2)
{
    if (is_heredoc_or_append(letter1, letter2))
        return (1);
    else if (is_redir_or_pipe(letter1))
        return (1);
    else
        return (0);
}

int preliminary_check(t_token *tokens)
{
    int token_list_size;

    token_list_size = get_list_size(tokens);
    if (token_list_size == 1 && is_special_character(tokens[0].value[0], tokens[0].value[1]))
    {
        syntax_error_message_display(NULL);
        return (1);
    }
    return (0);
}

void syntax_check(t_token *tokens, int *status)
{
    int i;

    i = 1;
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
