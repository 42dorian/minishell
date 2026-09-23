/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabdulla <dabdulla@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/22 01:44:20 by dabdulla          #+#    #+#             */
/*   Updated: 2026/09/23 09:38:17 by dabdulla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_path_words(char *s);
static void	free_strs(char **strs, int i);
static int	path_word_len(char *s);
static char	*copy_word(char *s, int len);

static int	count_path_words(char *s)
{
	int	count;
	int	i;

	count = 1;
	i = 0;
	while (s[i])
	{
		if (s[i] == ':')
			count++;
		i++;
	}
	return (count);
}

static void	free_strs(char **strs, int i)
{
	while (i >= 0)
	{
		free(strs[i]);
		i--;
	}
	free(strs);
}

static int	path_word_len(char *s)
{
	int	len;

	len = 0;
	while (s[len] && s[len] != ':')
		len++;
	return (len);
}

static char	*copy_word(char *s, int len)
{
	char	*str;

	if (len == 0)
		return (ft_strdup("."));
	str = ft_calloc(len + 1, sizeof(char));
	if (!str)
		return (NULL);
	ft_memcpy(str, s, len);
	return (str);
}

char	**custom_split_path(char *s)
{
	char	**strs;
	int		words;
	int		i;
	int		len;

	if (!s)
		return (NULL);
	words = count_path_words(s);
	strs = ft_calloc(words + 1, sizeof(char *));
	if (!strs)
		return (NULL);
	i = -1;
	while (++i < words)
	{
		strs[i] = copy_word(s, path_word_len(s));
		if (!strs[i])
			return (free_strs(strs, i - 1), NULL);
		s += len;
		if (*s == ':')
			s++;
	}
	return (strs);
}
