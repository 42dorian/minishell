/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabdulla <dabdulla@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 18:56:07 by dabdulla          #+#    #+#             */
/*   Updated: 2026/09/02 20:15:35 by dabdulla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int env(t_envs *env_list)
{
	t_envs *tmp;

	tmp = env_list;
	while (tmp)
	{
		ft_putstr_fd(tmp->key, 1);
		ft_putstr_fd("=", 1);
		ft_putstr_fd(tmp->value, 1);
		ft_putstr_fd("\n", 1);
		tmp = tmp->next;
	}
	return (0);
}
