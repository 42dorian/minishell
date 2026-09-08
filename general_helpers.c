/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabdulla <dabdulla@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 22:57:48 by dabdulla          #+#    #+#             */
/*   Updated: 2026/09/07 23:14:51 by dabdulla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_error(char *msg, char *cmd, char *arg, int fd)
{
	int len;
	char *buffer;

	len = ft_strlen("minishell: ") + ft_strlen(cmd) + ft_strlen(msg) + 3;
	if (arg)
		len += ft_strlen(arg) + ft_strlen(": ");
	buffer = ft_calloc(len + 1, sizeof(char));
	if (!buffer)
		return ;
	ft_strlcat(buffer, "minishell: ", len + 1);
	if (cmd)
	{
		ft_strlcat(buffer, cmd, len + 1);
		ft_strlcat(buffer, ": ", len + 1);
	}
	if (arg)
	{
		ft_strlcat(buffer, arg, len + 1);
		ft_strlcat(buffer, ": ", len + 1);
	}
	ft_strlcat(buffer, msg, len + 1);
	ft_strlcat(buffer, "\n", len + 1);
	write(fd, buffer, ft_strlen(buffer));
	free(buffer);
}
