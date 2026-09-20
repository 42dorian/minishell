/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   final_token_construct.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/20 20:10:30 by bguthy            #+#    #+#             */
/*   Updated: 2026/09/20 22:00:37 by bguthy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int get_len_of_total_token_struct(t_token *initial_tokens, t_envs *env_list, int *exit_code)
{
    int i;
    int len;
    int struct_size;
    char    *expanded_token;
    char    **split_token;

    i = 0;
    len = 0;
    struct_size = token_list_size(initial_tokens);
    while (i < struct_size)
    {
        if (dollar_in_word(initial_tokens[i].value))
        {
            expanded_token = handle_expansions(env_list, initial_tokens[i].value, exit_code);
            split_token = split_read_line(expanded_token);
            len += len_of_split_line(split_token);
            split_clean_up(split_token);
        }
        else
            len++;
        i++;
    }
    return (len);
}

int copy_token_node(t_token *new, t_token old)
{
    new->value = ft_strdup(old.value);
    if (!new->value)
        return (0);
    new->type = old.type;
    new->quoted = 0;
    return (1);
}

char    **split_token(char *line, t_envs *env_list, int *exit_code)
{
    char *expanded_line;
    char **split_expanded;
    
    expanded_line = handle_expansions(env_list, line, exit_code);
    if (!expanded_line)
        return (NULL);
    split_expanded = split_read_line(expanded_line);
    if (!split_expanded)
        return (free(expanded_line), NULL);
    return (split_expanded);
}

int add_to_final_struct(t_token *full_token, int *i, char **expanded_split)
{
    int split_len;
    int split_index;
    int counter;

    split_index = 0;
    counter = 0;
    split_len = len_of_split_line(expanded_split);
    while (counter < split_len)
    {
        full_token[*i].value = ft_strdup(expanded_split[split_index]);
        if (!full_token[*i].value)
            return (0);
        full_token[*i].type = token_word;
        full_token[*i].quoted = 0;
        (*i)++;
        split_index++;
        counter++;
    }
    return (1);
}

void    add_last_node_to_final_token_list(t_token *final_token)
{
    final_token->value = NULL;
    final_token->quoted = -1;
    final_token->type = token_invalid;
}

t_token *create_final_token_struct(t_token *tokens, t_envs *env_list, int *exit_code)
{
    int     prev_index;
    int     local_index;
    char    **expanded_split;
    t_token *final_token_list;
    int i;

    i = 0;
    prev_index = 0;
    local_index = 0;
    final_token_list = ft_calloc(sizeof(t_token), get_len_of_total_token_struct(tokens, env_list, exit_code) + 1);
    if (!final_token_list)
        return (0);
    while (tokens[prev_index].type != token_invalid)
    {
        if (dollar_in_word(tokens[prev_index].value))
        {
            expanded_split = split_token(tokens[prev_index].value, env_list, exit_code);
            if (!expanded_split)
                return (NULL);
            if (!add_to_final_struct(final_token_list, &local_index, expanded_split))
                return (NULL);
        }
        else if (!copy_token_node(&final_token_list[local_index], tokens[prev_index]))
            return (0);
        else
            local_index++;
        prev_index++;
    }
    add_last_node_to_final_token_list(&final_token_list[local_index]);
    return (final_token_list);
}
