/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabdulla <dabdulla@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:57:36 by dabdulla          #+#    #+#             */
/*   Updated: 2025/10/13 12:41:18 by dabdulla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	if (*s == (unsigned char)c)
		return ((char *)s);
	while (*s++)
	{
		if (*s == (unsigned char)c)
			return ((char *)s);
	}
	return (NULL);
}

/*#include <stdio.h>
#include <string.h>

int	main(void)
{
	const char *str = "Hello, World!";
	char *res1 = ft_strchr(str, 'H');
	char *std1 = strchr(str, 'H');
	printf("Test 1: %s | %s\n", res1, std1);
}*/