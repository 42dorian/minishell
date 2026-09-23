/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabdulla <dabdulla@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 12:57:21 by dabdulla          #+#    #+#             */
/*   Updated: 2026/09/23 23:51:34 by dabdulla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	event_hook(void)
{
	if (g_signal == SIGINT)
	{
		rl_done = 1;
		g_signal = 42;
		return (1);
	}
	return (0);
}

int	event_hook_main(void)
{
	if (g_signal == SIGINT)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		write(STDOUT_FILENO, "\n", 1);
		rl_redisplay();
		g_signal = 42;
		return (1);
	}
	return (0);
}

void	update_g_signal(t_shell *shell)
{
	if (g_signal == 42)
	{
		shell->status = 130;
		g_signal = 0;
	}
}
