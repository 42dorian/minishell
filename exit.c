#include "minishell.h"

static long long	ft_atoll(char *nptr);
static int			handle_exit_errors(t_cmds *cmds, t_shell *shell);
static void			print_exit_error(char *msg, t_cmds *cmd);
static int			valid_arg(t_cmds *cmd);

int	exit_bi(t_cmds *cmd, t_shell *shell)
{
	long long	exit_code;

	exit_code = 0;
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	exit_code = handle_exit_errors(cmd, shell);
	if (exit_code == 1)
		return (exit_code);
	if (exit_code == 0 || !(exit_code > 0 && exit_code <= 2))
		exit_code = ft_atoll(cmd->cmd[1]);
	exit((exit_code % 256));
}

long long	ft_atoll(char *nptr)
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

int	handle_exit_errors(t_cmds *cmd, t_shell *shell)
{
	if (!cmd->cmd[1])
		return (shell->status);
	if (cmd->cmd[2])
	{
		print_exit_error("too many arguments", cmd);
		return (1);
	}
	if (!valid_arg(cmd))
	{
		print_exit_error("numeric argument required", cmd);
		return (2);
	}
	return (0);
}

void	print_exit_error(char *msg, t_cmds *cmd)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd->cmd[0], STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	if (cmd->cmd[1] && !cmd->cmd[2])
	{
		ft_putstr_fd(cmd->cmd[1], STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putchar_fd('\n', STDERR_FILENO);
}

int	valid_arg(t_cmds *cmd)
{
	char	*tmp;

	tmp = cmd->cmd[1];
	while (*tmp == 32 || (*tmp >= 9 && *tmp <= 13))
		tmp++;
	if (*tmp == '\0')
		return (2);
	return (0);
}

int	valid_digit(t_cmds *cmd)
{
	int	i;

	i = 0;
	while (cmd->cmd[1][i])
	{
		if (!ft_isdigit(cmd->cmd[1][i]))
			return (2);
		i++;
	}
	return (0);
}

int	valid_long(char *str)
{
	int			is_negative;
	int			i;
	int			d;
	unsigned long long	v;

	is_negative = 0;
	i = 0;
	v = 0;
	if (str[i] == '-' || str[i] == '+')
		is_negative = (str[i++] == '-');
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (2);
		d = str[i] - '0';
		if (!is_negative)
		{
			if (v > LLONG_MAX / 10 || v == LLONG_MAX / 10 && d > LLONG_MAX % 10)
				return (2);
		}
		else
		{
			if (v > (unsigned long long)-(LLONG_MIN / 10) || (v == (unsigned long long)-(LLONG_MIN / 10) && d > -(LLONG_MIN
					% 10)))
				return (2);

		}
		v = (v * 10) + d;
		i++;
	}
	return (0);
}
