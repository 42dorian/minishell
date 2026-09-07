/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabdulla <dabdulla@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/05 17:36:01 by dabdulla          #+#    #+#             */
/*   Updated: 2026/09/07 23:37:28 by dabdulla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static long long	ft_atoll(char *nptr);
static int			check_long(int is_negative, long long v, int d);
static int			valid_long(char *str);

//free evth here before exit
int	exit_bi(t_cmds *cmd, t_shell *shell)
{
	long long	exit_code;

	exit_code = 0;
	ft_putstr_fd("exit\n", STDERR_FILENO);
	if (!cmd->cmd[1])
		exit(shell->status);
	if (valid_long(cmd->cmd[1]))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(cmd->cmd[1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		exit(2);
	}
	if (cmd->cmd[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	exit_code = ft_atoll(cmd->cmd[1]);
	exit((unsigned char)exit_code);
}

static long long	ft_atoll(char *nptr)
{
	int			i;
	int			sign;
	long long	value;

	value = 0;
	i = 0;
	sign = 1;
	while (nptr[i] == 32 || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] == '-')
	{
		sign = sign * -1;
		i++;
		if (nptr[i] == '+')
			return (0);
	}
	else if (nptr[i] == '+')
		i++;
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		value = value * 10 + nptr[i] - 48;
		i++;
	}
	return (value * sign);
}

static int	valid_long(char *str)
{
	int					is_negative;
	int					i;
	unsigned long long	v;

	is_negative = 0;
	i = 0;
	v = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
		is_negative = (str[i++] == '-');
	if (str[i] == '\0' || !ft_isdigit(str[i]))
		return (2);
	while (str[i] && ft_isdigit(str[i]))
	{
		if (check_long(is_negative, v, str[i] - '0'))
			return (2);
		v = (v * 10) + str[i] - '0';
		i++;
	}
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] != '\0')
		return (2);
	return (0);
}

static int	check_long(int is_negative, long long v, int d)
{
	if (!is_negative)
	{
		if (v > LLONG_MAX / 10 || (v == LLONG_MAX / 10 && d > LLONG_MAX % 10))
			return (2);
	}
	else
	{
		if (v > (unsigned long long)-(LLONG_MIN / 10)
			|| (v == (unsigned long long)-(LLONG_MIN / 10) && d > -(LLONG_MIN
					% 10)))
			return (2);
	}
	return (0);
}
