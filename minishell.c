/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguthy <bguthy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 19:02:10 by bguhty            #+#    #+#             */
/*   Updated: 2026/09/17 22:56:09 by bguthy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal = 0;

int	create_token_struct_and_remove_quotes(t_token *tokens, char **split_line)
{
	if (!create_token_struct(tokens, split_line))
	{
		clean_up_tokens_and_split_line(tokens, split_line);
		return (0);
	}
	if (!remove_quotes(tokens))
	{
		clean_up_tokens_and_split_line(tokens, split_line);
		return (0);
	}
	split_clean_up(split_line);
	return (1);
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
		return (NULL);
	tokens = ft_calloc(sizeof(t_token), (len_of_split_line(split_line) + 1));
	if (!tokens)
		return (split_clean_up(split_line), NULL);
	if (!create_token_struct_and_remove_quotes(tokens, split_line))
		return (NULL);
	return (tokens);
}

void	run_commands(t_shell *shell, t_token *tokens)
{
	int should_exec;

	should_exec = 1;
	if (syntax_check(tokens, &shell->status) == 1)
	{
		should_exec = 0;
		printf("here?");//error fix here
	}
	shell->cmds = build_cmds(tokens, shell->env_list, shell);
	free_tokens(tokens);
	if (!shell->cmds)
		return ;
	if (should_exec == 1)
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
	t_shell	shell;
	t_token	*tokens;

	tokens = NULL;
	(void)av;
	tokens = NULL;
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
