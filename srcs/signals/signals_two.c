/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_two.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 20:18:31 by aapadill          #+#    #+#             */
/*   Updated: 2024/12/21 20:18:33 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_exit_status(int status)
{
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (WEXITSTATUS(status));
}

void	hd_sig_handler(int sig)
{
	int	null_fd;

	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		null_fd = open("/dev/null", O_RDONLY);
		if (null_fd >= 0)
		{
			if (dup2(null_fd, STDIN_FILENO) < 0)
				free_and_exit();
			close(null_fd);
		}
		g_exit_status = 130;
	}
}

void	activate_hd_signal_handler(void)
{
	signal(SIGINT, hd_sig_handler);
	signal(SIGQUIT, hd_sig_handler);
}

void	restore_default_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	free_exit(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_exit_status = 130;
		exit(g_exit_status);
	}
}
