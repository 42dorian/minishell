/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabdulla <dabdulla@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/22 01:44:20 by dabdulla          #+#    #+#             */
/*   Updated: 2026/09/22 02:02:53 by dabdulla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int count_path_words(char *s);
static void	free_strs(char **strs, int i);
static int path_word_len(char *s);

static int count_path_words(char *s)
{
	int count;
	int i;

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

static int path_word_len(char *s)
{
	int len;

	len = 0;
	while(s[len] && s[len] != ':')
		len++;
	return (len);
}

char **custom_split_path(char *s)
{
	char **strs;
	int words;
	int i;
	int len;

	if (!s)
		return (NULL);
	words = count_path_words(s);
	strs = ft_calloc(words + 1, sizeof(char *));
	if (!strs)
		return (NULL);
	i = 0;
	while (i < words)
	{
		len = path_word_len(s);
		if (len == 0)
			strs[i] = ft_strdup(".");
		else
		{
			strs[i] = ft_calloc(len + 1, sizeof(char));
			if (!strs[i])
				return (free_strs(strs, i - 1), NULL);
			ft_memcpy(strs[i], s, len);
			strs[i][len] = '\0';
		}
		s += len;
		if (*s == ':')
			s++;
		i++;
	}
	strs[i] = NULL;
	return (strs);
}
