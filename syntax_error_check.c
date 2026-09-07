/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error_check.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guthybarnakoppany <guthybarnakoppany@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 16:08:24 by bguhty            #+#    #+#             */
/*   Updated: 2026/09/06 18:56:40 by guthybarnak      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int     syntax_error_message_display(const char *token_value)
{
    if (!token_value)
        printf("minishell: syntax error near the token `newline'\n");
    else
        printf("minishell: syntax error near the token `%s'\n", token_value);
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

void syntax_check(t_token *tokens)
{
    int i;
    
    i = 1;
    while (tokens[i].value)
    {
        if (pipe_check(tokens, i))
            break ;
        else if (heredoc_check(tokens, i))
            break ;
        else if (redir_check(tokens, i))
            break ;
        i++;
    }
}
