/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 19:02:10 by bguhty            #+#    #+#             */
/*   Updated: 2026/09/17 14:48:46 by bguthy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal = 0;

int	determine_quote_type(char letter, int quote_type)
{
	if (quote_type == SINGLE_QUOTE && letter == SINGLE_QUOTE)
		quote_type = 0;
	else if (quote_type == DOUBLE_QUOTE && letter == DOUBLE_QUOTE)
		quote_type = 0;
	else if (quote_type == 0 && letter == SINGLE_QUOTE)
		quote_type = SINGLE_QUOTE;
	else if (quote_type == 0 && letter == DOUBLE_QUOTE)
		quote_type = DOUBLE_QUOTE;
	return (quote_type);
}

int	set_quote_type(int *quote_type, const char letter)
{
	if (is_single_quote(letter) && *quote_type == 0)
	{
		*quote_type = SINGLE_QUOTE;
		return (1);
	}
	if (is_double_quote(letter) && *quote_type == 0)
	{
		*quote_type = DOUBLE_QUOTE;
		return (1);
	}
	if (is_single_quote(letter) && *quote_type == SINGLE_QUOTE)
	{
		*quote_type = 0;
		return (1);
	}
	if (is_double_quote(letter) && *quote_type == DOUBLE_QUOTE)
	{
		*quote_type = 0;
		return (1);
	}
	return (0);
}

t_token	*minishell(const char *read_line, t_envs *env_list, int *status)
{
	int		i;
	t_token	*tokens;
	char	**split_line;
	char	*expanded_line;

	i = 0;
	if (empty_string_and_unclosed_quote_check(read_line, status))
		return (NULL);
	expanded_line = handle_expansions(env_list, read_line, status);
	if (!expanded_line)
		return (NULL);
	split_line = split_read_line(expanded_line);
	if (!split_line)
		return (free(expanded_line), NULL);
	tokens = malloc(sizeof(t_token) * (word_counter(expanded_line) + 1));
	if (!tokens)
		return (split_clean_up(split_line, word_counter(expanded_line)),
			free(expanded_line), NULL);
	if (!create_token_struct(tokens, split_line))
		return (clean_up_tokens_and_split_line(tokens, split_line),
			free(expanded_line), NULL);
	split_clean_up(split_line, word_counter(expanded_line));
	free(expanded_line);
	if (!remove_quotes(tokens))
		return (free_tokens(tokens), NULL);
	if ((syntax_check(tokens, status)) == 1 && *status == 2)
		return (free_tokens(tokens), NULL);
	return (tokens);
}

void	run_commands(t_shell *shell, t_token *tokens)
{
	shell->cmds = build_cmds(tokens, shell->env_list, shell);
	free_tokens(tokens);
	if (!shell->cmds)
		return ;
	shell->status = execute_cmds(shell);
	free_cmds(&shell->cmds);
}

void	main_loop(t_shell *shell, t_token *tokens)
{
	char	*line;

	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			break ;
		if (line[0] != '\0')
			add_history(line);
		if (WTERMSIG(g_signal) != 0)
		{
			shell->status = 128 + WTERMSIG(g_signal);
			g_signal = 0;
		}
		tokens = minishell(line, shell->env_list, &shell->status);
		free(line);
		if (tokens)
			run_commands(shell, tokens);
	}
	rl_clear_history();
	ft_putstr_fd("exit\n", STDERR_FILENO);
	free_all_and_exit(shell, shell->status);
}

int	main(int ac, char **av, const char **envp)
{
	char	*line;
	t_shell	shell;
	t_token	*tokens;

	(void)av;
	if (ac != 1)
	{
		ft_putstr_fd("minishell doesn't take arguments\n", STDERR_FILENO);
		return (1);
	}
	ft_bzero(&shell, sizeof(shell));
	if (!add_envp_to_list(&shell.env_list, envp))
		return (1);
	init_interactive_signals();
	main_loop(&shell, tokens);
	return (shell.status);
}
