/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabdulla <dabdulla@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 14:10:25 by dabdulla          #+#    #+#             */
/*   Updated: 2025/10/07 14:18:48 by dabdulla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putendl_fd(char *s, int fd)
{
	int	i;

	i = 0;
	if (!s)
		return ;
	while (s[i])
		i++;
	write(fd, s, i);
	write(fd, "\n", 1);
}

/*#include <fcntl.h>
int	main(void)
{
	char *s = "hello";
	int fd;

	fd = open("TEST.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	ft_putendl_fd(s, fd);
	close(fd);
}*/
