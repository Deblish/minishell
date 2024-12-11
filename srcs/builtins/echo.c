# include "minishell.h"

/*
int	ft_echo(int ac, char **av)
{
	int	nl;
	int	i;

	nl = 1;
	if (ft_strcmp(av[0], "echo"))
		return (1);
	i = 1;
	if (ac > 1 && !ft_strcmp(av[1], "-n"))
	{
		nl = 0;
		i = 2;
	}
	while (i < ac)
	{
		ft_putstr_fd(av[i], 1);
		if (i != ac - 1)
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (ac >= 1 && nl)
		ft_putchar_fd('\n', 1);
	return (0);
}*/

int	ft_echo(char **av)
{
	int	nl;
	int	i;

	nl = 1;
	i = 0;
	if (!ft_strcmp(av[0], "echo"))
		return (1);
	if (av[1] && !ft_strcmp(av[1], "-n"))
	{
		nl = 0;
		i = 2;
	}
	while (av[i])
	{
		ft_putstr_fd(av[i], 1);
		if (av[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (nl) //will only happen if nl flag is on
		ft_putchar_fd('\n', 1);
	return (0);
}