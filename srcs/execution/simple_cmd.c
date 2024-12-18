/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 10:53:03 by aapadill          #+#    #+#             */
/*   Updated: 2024/12/06 10:53:11 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"
#include "execution.h"

int	get_redirect_type(t_ast_node *io_redirect_node)
{
	t_ast_node	*child;
	t_ast_node	*op_node;

	child = io_redirect_node->children[0];
	if (child->type == IO_FILE)
	{
		op_node = child->children[0];
		if (op_node->token->type == RED_FO)
			return (RED_FO);
		else if (op_node->token->type == RED_TO)
			return (RED_TO);
		else if (op_node->token->type == DGREAT)
			return (DGREAT);
	}
	else if (child->type == IO_HERE)
	{
		op_node = child->children[0];
		if (op_node->token->type == DLESS)
			return (DLESS);
	}
	return (-1);
}

char	*get_filename(t_ast_node *io_redirect_node)
{
	t_ast_node	*child;
	t_ast_node	*filename;
	t_ast_node	*here_end;
	t_ast_node	*word;

	child = io_redirect_node->children[0];
	if (child->type == IO_FILE)
	{
		filename = child->children[1];
		word = filename->children[0];
		return (word->token->value);
	}
	else if (child->type == IO_HERE)
	{
		here_end = child->children[1];
		word = here_end->children[0];
		return (word->token->value);
	}
	return (NULL);
}

char	*get_here_end_word(t_ast_node *io_redirect)
{
	t_ast_node	*io_here;
	t_ast_node	*here_end;
	t_ast_node	*word;

	io_here = io_redirect->children[0];
	here_end = io_here->children[1];
	word = here_end->children[0];
	return (word->token->value);
}

void	gather_redirects(t_ast_node *node, t_exec_info *info)
{
	int		type;
	char	*end_word;
	int		pipefd[2];
	char	*line;
	t_redir_info	*redir_info;
	int		i;

	if (!node)
		return ;

	if (node->type == IO_REDIRECT)
	{
		type = get_redirect_type(node);
		if (type == DLESS)
		{
			end_word = get_here_end_word(node);
			if (pipe(pipefd) == -1)
			{
				perror("pipe");
				return ;
				//exit(1); //is this the correct exit code?
			}
			//stops until here_end word is found
			signal(SIGINT, here_doc_sig);
			while (1)
			{
				line = readline("heredoc> ");
				if (!line || ft_strcmp(line, end_word) == 0)
				{
					g_exit_status = 0;
					free(line);
					break ;
				}
				ft_putendl_fd(line, pipefd[1]);
				free(line);
			}
			close(pipefd[1]);
			info->heredoc_fd = pipefd[0];
		}
		else if (type == RED_FO || type == RED_TO || type == DGREAT)
		{
			redir_info = gc_alloc(sizeof(t_redir_info));
			redir_info->type = type;
			redir_info->filename = env_expander(get_filename(node)); //kinda hardcoded
			ft_lstadd_back(&info->redir_list, ft_lstnew(redir_info)); //switch to gc
		}
		return ;
	}
	i = 0;
	while (i < node->child_count)
	{
		gather_redirects(node->children[i], info);
		i++;
	}
}

void	apply_normal_redirections(t_list *normal_redirects)
{
	t_redir_info	*redir_info;
	int				fd;

	while (normal_redirects)
	{
		redir_info = normal_redirects->content;
		fd = -1;
		if (redir_info->type == RED_FO) //<
			fd = open(redir_info->filename, O_RDONLY);
		else if (redir_info->type == RED_TO) //>
			fd = open(redir_info->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (redir_info->type == DGREAT) //>>
			fd = open(redir_info->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd < 0)
		{
			perror(redir_info->filename);
			//return ; //or exit?
			exit(1); //is this the correct exit code?
		}
		if (redir_info->type == RED_FO)
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		close(fd);
		normal_redirects = normal_redirects->next;
	}
}

bool	unclosed_quotes(char *str)
{
	int		i;
	int		dq;
	int		sq;
	bool	unclosed;

	i = 0;
	dq = 0;
	sq = 0;
	while (str[i])
	{
		if (str[i] == '\"')
			dq++;
		else if (str[i] == '\'')
			sq++;
		i++;
	}
	unclosed = (dq % 2);
	if (!unclosed)
		unclosed = (sq % 2);
	return (unclosed);
}

//env var format: \$[A-Za-z_][A-Za-z0-9_]*
char	*env_expander(char *str)
{
	char	*pre;
	char	*env_var;
	char	*value;
	char	*tmp;
	int		in_sq;
	int		in_dq;
	int		i;
	int		start;
	char	single_char[2];

	pre = gc_strdup("");
	in_sq = 0;
	in_dq = 0;
	i = 0;
	single_char[1] = '\0';
	while (str[i])
	{
		if (str[i] == '"' && !in_sq)
		{
			in_dq = !in_dq;
			i++;
			continue;
		}
		else if (str[i] == '\'' && !in_dq)
		{
			in_sq = !in_sq;
			i++;
			continue;
		}
		//check for a variable expansion
		if (str[i] == '$' && !in_sq)
		{
			if (str[i + 1] == '?')
			{
				//handle last exit code, hardcoded for now to 42
				//char *exit_code_str = ft_itoa(g->last_exit_status);
				tmp = gc_strjoin(pre, "42");
				gc_free(pre);
				//gc_free(exit_code_str);
				pre = tmp;
				i += 2; //past "$?"
				continue;
			}
			else if ((str[i + 1] == '_' || ft_isalpha(str[i + 1])))
			{
				start = i + 1;
				i += 1;
				while (ft_isalnum(str[i]) || str[i] == '_')
					i++;
				env_var = gc_substr(str, start, i - start);
				value = get_env(env_var);
				gc_free(env_var);

				if (value)
				{
					tmp = gc_strjoin(pre, value);
					gc_free(pre);
					gc_free(value);
					pre = tmp;
				}
				//if no value, we remove the variable and not append anything
				continue;
			}
			else
			{
				//append one character (with a null terminator by default)
				single_char[0] = str[i];
				tmp = gc_strjoin(pre, single_char);
				gc_free(pre);
				pre = tmp;
				i++;
				continue;
			}
		}
		//normal character that is not a quote or $
		single_char[0] = str[i];
		tmp = gc_strjoin(pre, single_char);
		gc_free(pre);
		pre = tmp;
		i++;
	}
	return (pre);
}

void	construct_cmd(t_ast_node *node, t_list **words)
{
	t_list	*cmd_elem;
	int		i;

	if (!node)
		return ;

	if (node->type == IO_REDIRECT || node->type == IO_FILE || node->type == IO_HERE || node->type == FILENAME)// || node->type == HERE_END)
		return ;

	if (node->type == WORD)
	{
		//remove_quotes(node->token->value, '\"');
		//printf("\ttype: %s\n", get_symbol_name(node->token->type));
		//printf("\tvalue: %s\n", node->token->value);
		//printf("\tstate: %i\n", node->token->state);
		//printf("\texpanded: %s\n", env_expander(node->token->value));
		//cmd_elem = ft_lstnew(node->token->value);
		cmd_elem = ft_lstnew(env_expander(node->token->value));
		//if (!cmd_elem)
		ft_lstadd_back(words, cmd_elem); //switch to gc
		return ;
	}
	i = 0;
	while (i < node->child_count)
	{
		construct_cmd(node->children[i], words);
		i++;
	}
}

char	**build_argv(t_ast_node *simple_command)
{
	t_list	*words;
	t_list	*tmp;
	int		i;
	char	**argv;

	i = 0;
	words = NULL;
	construct_cmd(simple_command, &words);
	tmp = words;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	argv = gc_alloc(sizeof(char *) * (i + 1));
	if (!argv)
	{
		//free words linked list
		return (NULL);
	}
	i = 0;
	while (words)
	{
		argv[i] = words->content;
		i++;
		words = words->next;
	}
	argv[i] = NULL;
	//free words linked list
	if (get_debug())
	{
		printf("words linked list:\n");
		i = 0;
		while (argv[i])
		{
			printf("%s\n", argv[i]);
			i++;
		}
		printf("-----------------your expected stuff starts this line\n");
	}
	return (argv);
}

static int	is_cmd_already_path(char *cmd)
{
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) == 0)
			return (1);
		else
		{
			ft_putstr_fd("./microshell: ", STDERR_FILENO);
			perror(cmd);
			gc_free_all();
			exit(EXIT_CMD_NOT_FOUND);
		}
	}
	return (0);
}

char	*look_for_cmd(char *cmd, char **paths)
{
	int		i;
	char	*aux;
	char	*triable_path;

	i = 0;
	while (paths[i])
	{
		aux = gc_strjoin(paths[i], "/");
		triable_path = gc_strjoin(aux, cmd);
		if (access(triable_path, F_OK | X_OK) == 0)
		{
			g_exit_status = 0;
			return (triable_path);
		}
		i++;
	}
	//ft_putstr_fd("./microshell: ", STDERR_FILENO);
	//ft_putstr_fd(cmd, STDERR_FILENO);
	//ft_putendl_fd(": command not found", STDERR_FILENO);
	//gc_free_all(); //possible due to being in a child process?
	//exit(EXIT_CMD_NOT_FOUND);
	return (NULL);
}

void	execute_simple_cmd(t_ast_node *simple_cmd)
{
	char	**argv;
	pid_t	pid;
	char	*path;
	char	**paths;
	int		status;
	int		n;
	t_exec_info	info;
	int original_in;
	int	original_out;
	//int		exit_builtin;

	info.heredoc_fd = -1;
	info.redir_list = NULL;
	gather_redirects(simple_cmd, &info);
	argv = build_argv(simple_cmd);
	//if (!argv) //either no command or unclosed quotes
	//	return ;
	original_in = dup(STDIN_FILENO);
	original_out = dup(STDOUT_FILENO);
	if (!original_in || !original_out)
	{
		perror("dup");
		return ;
	}
	if (argv && is_builtin(argv[0]))
	{
		if (info.heredoc_fd != -1)
		{
			dup2(info.heredoc_fd, STDIN_FILENO);
			close(info.heredoc_fd);
		}
		apply_normal_redirections(info.redir_list);
		//exit_builtin = execute_builtin(argv);
		g_exit_status = execute_builtin(argv);
		dup2(original_in, STDIN_FILENO);
		dup2(original_out, STDOUT_FILENO);
		close(original_in);
		close(original_out);
		//exit(exit_builtin);
		return ;
	}
	else if (argv)
	{
		pid = fork();
		if (pid < 0)
		{
			perror("fork");
			return ; //not sure if we should return or exit
		}
		else if (pid == 0) //child
		{
			ignore_signals();
			if (info.heredoc_fd != -1)
			{
				dup2(info.heredoc_fd, STDIN_FILENO);
				close(info.heredoc_fd);
			}
			apply_normal_redirections(info.redir_list);
			path = get_env("PATH");
			if (!path)
			{
				ft_putstr_fd("microshell: ", 2);
				ft_putstr_fd(argv[0], 2);
				ft_putendl_fd(": No such file or directory", 2);
				exit(127); //is this the correct exit code?
			}
			if (is_cmd_already_path(argv[0]))
				path = argv[0];
			else
			{
				paths = gc_split(path, ':', &n);
				path = look_for_cmd(argv[0], paths);
				gc_free_array(n, (void **)paths);
			}
			if (!path)
			{
				ft_putstr_fd(argv[0], 2);
				ft_putendl_fd(": command not found", 2);
				exit(127); //is this the correct exit code?
			}
			if (execve(path, argv, g_env->envp) == -1)
			{
				perror("execve");
				exit(126); //is this the correct exit code?
			}
		}
		else
		{
			activate_signal_handler();
			waitpid(pid, &status, 0);
			//if (WIFEXITED(status))
			//exit(WEXITSTATUS(status));
			//	g_env->last_exit_status = WEXITSTATUS(status);
		}
	}
	//gc_free_array((void **)argv, //length of argv);
}

void	execute_simple_piped_cmd(char **argv)
{
	char	*path;
	char	**paths;
	int		status;
	int		n;

	if (!argv || !argv[0])
		exit(EXIT_FAILURE);
	
	if (is_builtin(argv[0]))
	{
		status = execute_builtin(argv);
		exit(status);
	}

	ignore_signals();
	path = get_env("PATH");
	if (is_cmd_already_path(argv[0]))
		path = argv[0];
	else
	{
		if (!path)
		{
			ft_putstr_fd("microshell: ", 2);
			ft_putstr_fd(argv[0], 2);
			ft_putendl_fd(": No such file or directory", 2);
			exit(127); //is this the correct exit code?
		}
		paths = gc_split(path, ':', &n);
		path = look_for_cmd(argv[0], paths);
		gc_free_array(n, (void **)paths);
	}

	if (!path)
	{
		ft_putstr_fd(argv[0], 2);
		ft_putendl_fd(": command not found", 2);
		exit(127); //is this the correct exit code?
	}
	if (execve(path, argv, g_env->envp) == -1)
	{
		perror("execve");
		exit(126); //is this the correct exit code?
	}
	//this should not happen
}