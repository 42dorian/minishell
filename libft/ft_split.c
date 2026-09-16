/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabdulla <dabdulla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 15:59:08 by dabdulla          #+#    #+#             */
/*   Updated: 2025/10/11 09:32:32 by dabdulla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(char const *s, char c)
{
	int	count;
	int	next_word;

	if (!s)
		return (0);
	count = 0;
	next_word = 0;
	while (*s)
	{
		if (*s == c)
			next_word = 0;
		else if (!next_word)
		{
			next_word = 1;
			count++;
		}
		s++;
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

static int	word_len(char const *s, char c)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s[i])
	{
		if (s[i] != c)
			j++;
		else if (s[i] == c)
			return (j);
		i++;
	}
	return (j);
}

static char	**copy_strs(char **strs, char const *s, char c, int words)
{
	int	i;

	i = 0;
	while (i < words)
	{
		if (*s == c)
			s++;
		else
		{
			strs[i] = malloc(word_len(s, c) + 1);
			if (!strs[i])
			{
				free_strs(strs, --i);
				return (NULL);
			}
			ft_memcpy(strs[i], s, word_len(s, c));
			strs[i][word_len(s, c)] = '\0';
			s += word_len(s, c);
			i++;
		}
	}
	strs[i] = NULL;
	return (strs);
}

char	**ft_split(char const *s, char c)
{
	char	**strs;
	int		words;

	words = count_words(s, c);
	strs = malloc(sizeof(char *) * (words + 1));
	if (!strs)
		return (NULL);
	strs = copy_strs(strs, s, c, words);
	return (strs);
}

// #include <stdio.h>
// int main(void)
// {
// 	int i;

// 	i = 0;
// 	char *s1;
// 	s1 = NULL;
// 	char c = NULL;

//     char **strs = ft_split("test testset set set ", c);
//     printf("Test: \n");
// 	while (strs[i])
// 	{
// 		printf("[%i][%s]\n", i, strs[i]);
// 		i++;
// 	}
// 	i = 0;
// 	while (strs[i])
// 	{
// 		free(strs[i]);
// 		i++;
// 	}
// 	free(strs);
//     return (0);
// }
