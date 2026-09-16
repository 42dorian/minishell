/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabdulla <dabdulla@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 15:03:02 by dabdulla          #+#    #+#             */
/*   Updated: 2025/10/09 12:46:53 by dabdulla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	dst_size;
	size_t	src_size;
	size_t	last;

	dst_size = ft_strlen(dst);
	src_size = ft_strlen(src);
	last = (size - dst_size) - 1;
	i = 0;
	if (size <= dst_size)
		return (size + src_size);
	if (!size)
		return (dst_size + src_size);
	while (i < last && src[i])
	{
		dst[dst_size + i] = src[i];
		i++;
	}
	dst[dst_size + i] = '\0';
	return (dst_size + src_size);
}

/*#include <bsd/string.h> // use -lbsd flag when compiling
#include <stdio.h>

int	main(void)
{
	char d1[20] = "Hello ", d1_std[20] = "Hello ";
	const char *s1 = "World!";
	size_t r1 = ft_strlcat(d1, s1, sizeof(d1));
	size_t r1_std = strlcat(d1_std, s1, sizeof(d1));
	printf("Test 1: %s (%zu) | %s (%zu)\n", d1, r1, d1_std, r1_std);

	char d2[5] = "Hi", d2_std[5] = "Hi";
	const char *s2 = "There";
	size_t r2 = ft_strlcat(d2, s2, sizeof(d2));
	size_t r2_std = strlcat(d2_std, s2, sizeof(d2));
	printf("Test 2: %s (%zu) | %s (%zu)\n", d2, r2, d2_std, r2_std);

	char d3[15] = "", d3_std[15] = "";
	const char *s3 = "Test";
	size_t r3 = ft_strlcat(d3, s3, sizeof(d3));
	size_t r3_std = strlcat(d3_std, s3, sizeof(d3));
	printf("Test 3: %s (%zu) | %s (%zu)\n", d3, r3, d3_std, r3_std);

	char d4[15] = "AlreadyFull", d4_std[15] = "AlreadyFull";
	const char *s4 = "Extra";
	size_t r4 = ft_strlcat(d4, s4, 5);
	size_t r4_std = strlcat(d4_std, s4, 5);
	printf("Test 4: %s (%zu) | %s (%zu)\n", d4, r4, d4_std, r4_std);

	return (0);
}*/