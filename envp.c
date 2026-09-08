/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabdulla <dabdulla@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 11:53:57 by dabdulla          #+#    #+#             */
/*   Updated: 2026/09/02 18:46:49 by dabdulla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*build_env_string(char *key, char *value);
static void	free_strs(char **strs, int i);

static void	free_strs(char **strs, int i)
{
	while (i >= 0)
	{
		free(strs[i]);
		i--;
	}
	free(strs);
}

char	**create_envp(t_envs *env_list)
{
	int		lst_size;
	int		i;
	t_envs	*tmp;
	char	**envp;

	envp = NULL;
	i = 0;
	lst_size = ft_lstsize(env_list);
	tmp = env_list;
	envp = ft_calloc(lst_size + 1, sizeof(char *));
	if (!envp)
		return (NULL);
	while (i < lst_size)
	{
		envp[i] = build_env_string(tmp->key, tmp->value);
		if (!envp[i])
			return (free_strs(envp, --i), NULL);
		tmp = tmp->next;
		i++;
	}
	envp[i] = NULL;
	return (envp);
}

static char	*build_env_string(char *key, char *value)
{
	char	*tmp;
	int		key_len;
	int		value_len;
	int		total_len;

	key_len = ft_strlen(key);
	value_len = ft_strlen(value);
	total_len = key_len + value_len + 2;
	tmp = ft_calloc(total_len, sizeof(char));
	if (!tmp)
		return (NULL);
	ft_strlcpy(tmp, key, total_len);
	ft_strlcat(tmp, "=", total_len);
	if (value)
		ft_strlcat(tmp, value, total_len);
	return (tmp);
}
