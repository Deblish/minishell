/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 21:39:14 by aapadill          #+#    #+#             */
/*   Updated: 2024/12/01 22:06:05 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include <fcntl.h> //open()
# include "minishell.h"

/* -------------------------------------------------------------------------- */
/*                                  PARSING                                   */
/* -------------------------------------------------------------------------- */

typedef enum e_action
{
	SHIFT = 0,
	REDUCE = 1,
	ACCEPT = 2,
	GOTO = -1,
}				t_action;

typedef enum e_non_terminals
{
	ACCEP = 9,
	PIPE_SEQ = 10,
	SIMPLE_CMD = 11,
	CMD_NAME = 12,
	CMD_WORD = 13,
	CMD_PREFIX = 14,
	CMD_SUFFIX = 15,
	IO_REDIRECT = 16,
	IO_FILE = 17,
	IO_HERE = 19,
	FILENAME = 21,
	HERE_END = 23,
	DEFAULT = -1,
}				t_rules;

typedef struct s_entry
{
	int	state;
	int	token_type;
	int	action;
	int	go_to;
	int	reduce;
}		t_entry;

/* -------------------------------------------------------------------------- */
/*                                   STACK                                    */
/* -------------------------------------------------------------------------- */

typedef struct s_ast_node
{
	int					type;
	t_token_node		*token;
	struct s_ast_node	**children;
	int					child_count;
}	t_ast_node;

typedef struct s_node
{
	int				value;
	t_ast_node		*ast_node;
	struct s_node	*next;
}	t_node;

typedef struct s_stack
{
	t_node	*top;
	size_t	size;
}	t_stack;

/* -------------------------------------------------------------------------- */
/*                                PROTOTYPES                                  */
/* -------------------------------------------------------------------------- */

//debug_parser.c
void		print_ast(t_ast_node *node, int indent);
void		print_stacks(t_stack *stack, t_token_stack *tokens);
void		print_stack(t_stack *stack, char *name);
void		print_tokens(t_token_stack *tokens, char *name);

//lookups.c
t_entry		*non_terminal_lookup(t_entry **table, int state, int non_terminal);
t_entry		*actual_lookup(t_entry **table, int state, int token);
t_entry		*table_lookup(t_stack *stack,
				t_token_stack *tokens, t_entry **table);

//actions.c
int			action_shift(t_stack *stack, t_entry *entry, t_token_stack *tokens);
int			action_reduce(t_stack *stack, t_entry *entry, t_entry **table);
int			action_accept(void);
int			action_goto(t_stack *stack, t_entry *entry);

//ast.c
t_ast_node	**children_alloc_and_fill(t_stack *stack, int reduce);
t_ast_node	*init_ast_node(int type, t_token_node *token,
				t_ast_node **children, int child_count);
t_ast_node	*get_ast_root(t_stack *stack);

//parsing.c
int			parser(void);

//stack_operations.c
void		push(t_stack *stack, t_node *new_node);
t_node		*pop(t_stack *stack);

//stack_utils.c
t_node		*init_stack_node(t_ast_node *ast_node);
t_node		*init_node(int value);
//int		free_stack(t_stack *stack, int send_error);

//grammar_utils.c
bool		is_token(char ch, char next);
t_entry		*create_entry(char *line);
t_entry		**create_table(void);
int			get_non_terminal(int rule_number);
const char	*get_symbol_name(int type);

#endif
