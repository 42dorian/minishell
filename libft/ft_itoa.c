/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guthybarnakoppany <guthybarnakoppany@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 11:27:11 by dabdulla          #+#    #+#             */
/*   Updated: 2026/09/08 12:26:03 by guthybarnak      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	int_len(int n)
{
	int		i;
	long	long_n;

	long_n = n;
	i = 0;
	if (long_n == 0)
		i++;
	if (long_n < 0)
	{
		long_n = long_n * -1;
		i++;
	}
	while (long_n > 0)
	{
		long_n = long_n / 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int *n)
{
	char	*str;
	int		len;
	long	long_n;

	len = int_len(*n);
	str = malloc(len + 1);
	if (!str)
		return (NULL);
	long_n = *n;
	if (long_n == 0)
		str[0] = '0';
	if (long_n < 0)
	{
		long_n = long_n * -1;
		str[0] = '-';
	}
	while (long_n > 0)
	{
		str[--len] = (long_n % 10) + '0';
		long_n /= 10;
	}
	str[int_len(*n)] = '\0';
	return (str);
}
