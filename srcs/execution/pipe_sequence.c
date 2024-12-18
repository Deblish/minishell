/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_sequence.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 09:39:13 by aapadill          #+#    #+#             */
/*   Updated: 2024/12/10 09:39:14 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"
#include "execution.h"

t_ast_node	**get_simple_cmds(t_ast_node *node, int *count)
{
	t_ast_node	**simple_cmds;
	t_ast_node	**left_cmds;
	int			i;
	int	left_count;

	//just one simple command
	//pipe_sequence -> simple_command
	if (node->child_count == 1) //kinda hardcoded, fits the grammar though
	{
		simple_cmds = gc_alloc(sizeof(t_ast_node *));
		simple_cmds[0] = node->children[0];
		*count = 1;
		return (simple_cmds);
	}
	//more than one simple command
	//pipe_sequence -> simple_command PIPE pipe_sequence
	else
	{
		left_cmds = get_simple_cmds(node->children[0], &left_count);
		simple_cmds = gc_alloc(sizeof(t_ast_node *) * (left_count + 1));
		i = 0;
		while (i < left_count)
		{
			simple_cmds[i] = left_cmds[i];
			i++;
		}
		simple_cmds[left_count] = node->children[2];
		*count = left_count + 1;
		return (simple_cmds);
	}
}
void	execute_pipeline(t_ast_node **commands, int cmd_count)
{
	int pipe_count;
	int	pipefds[2 * (cmd_count - 1)];
	int	i;
	int	j;
	pid_t	*pid;
	int		status;

	i = 0;
	pipe_count = cmd_count - 1;
	pid = gc_alloc(pipe_count * sizeof(pid_t));
	//create pipes
	while (i < pipe_count)
	{
		if (pipe(pipefds + i * 2) < 0)
		{
			perror("pipe");
			exit(EXIT_FAILURE); //is this the correct exit code
		}
		i++;
	}
	ignore_signals();
	i = 0;
	while (i < cmd_count)
	{
		//leaving in here for now but separate into a function
		t_exec_info info;
		char **argv;
		info.heredoc_fd = -1;
        info.redir_list = NULL;

        //before forking
        gather_redirects(commands[i], &info);

        //build argv
        argv = build_argv(commands[i]);

		pid[i] = fork();
		if (pid[i] < 0)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else if (pid[i] == 0) //child
		{
			//if not first command
			if (i > 0)
			{
				if (dup2(pipefds[(i-1) * 2], STDIN_FILENO) < 0)
				{
					perror("dup2");
					exit(EXIT_FAILURE);
				}
			}
			//if not last command
			if (i < cmd_count - 1)
			{
				if (dup2(pipefds[i * 2 + 1], STDOUT_FILENO) < 0)
				{
					perror("dup2");
					exit(EXIT_FAILURE);
				}
			}
			//close all pipes
			j = 0;
			while (j < pipe_count * 2)
			{
				close(pipefds[j]);
				j++;
			}
			//apply heredoc
            if (info.heredoc_fd != -1)
            {
                if (dup2(info.heredoc_fd, STDIN_FILENO) < 0)
                {
                    perror("dup2 heredoc");
                    exit(EXIT_FAILURE);
                }
                close(info.heredoc_fd);
            }
			apply_normal_redirections(info.redir_list);
			execute_simple_piped_cmd(argv);
			exit(EXIT_FAILURE);
		}
		//parent
		i++;
	}
	//parent
	//close all pipes
	j = 0;
	while (j < pipe_count * 2)
	{
		close(pipefds[j]);
		j++;
	}
	//wait for all children
	i = 0;
	while (i < cmd_count)
	{
		waitpid(pid[i], &status, 0);
		if (WIFEXITED(status))
		{
			g_exit_status = WEXITSTATUS(status);
			if (g_exit_status == 1)
				g_exit_status = 127;
		}
		else if (WIFSIGNALED(status))
		{
			int signal_num = WTERMSIG(status);
			if (signal_num == SIGINT)
			{
				g_exit_status = 130;
			}
		}
		i++;
	}
	activate_signal_handler();
}

void	execute_pipe_seq(t_ast_node *node)
{
	int	count;
	t_ast_node	**simple_cmds;

	simple_cmds = get_simple_cmds(node, &count);
	if (count == 1)
	{
		execute_simple_cmd(simple_cmds[0]);
	}
	else
	{
		//execute multiple simple commands
		execute_pipeline(simple_cmds, count);
	}
	//free?
}
