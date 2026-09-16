/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabdulla <dabdulla@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 16:09:00 by dabdulla          #+#    #+#             */
/*   Updated: 2025/10/13 12:44:14 by dabdulla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*str;

	str = (unsigned char *)s;
	while (n-- > 0)
	{
		if (*str == (unsigned char)c)
			return (str);
		str++;
	}
	return (NULL);
}

/*#include <stdio.h>
#include <string.h>

int	main(void)
{
	const char str[] = "Hello, World!";

	void *res1 = ft_memchr(str, 'H', ft_strlen(str));
	void *std1 = memchr(str, 'H', ft_strlen(str));
	printf("ft_memchr: %s | memchr:  %s\n", (char*)res1, (char*)std1);

	return (0);
}*/