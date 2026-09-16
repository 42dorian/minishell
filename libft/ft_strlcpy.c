/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabdulla <dabdulla@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 13:01:59 by dabdulla          #+#    #+#             */
/*   Updated: 2025/10/13 12:43:49 by dabdulla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (size > 0)
	{
		while (src[i] && i < (size - 1))
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	return (ft_strlen(src));
}

/*#include <stdio.h>
#include <bsd/string.h>

int	main(void)
{
	char d1[20]; char d1_std[20];
	size_t r1 = ft_strlcpy(d1, "Hello, World!", sizeof(d1));
	size_t r1_std = strlcpy(d1_std, "Hello, World!", sizeof(d1_std));
	printf("Test 1: %s (%zu) | %s (%zu)\n", d1, r1, d1_std, r1_std);

}*/