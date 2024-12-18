/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 21:37:33 by aapadill          #+#    #+#             */
/*   Updated: 2024/11/30 14:43:53 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "minishell.h"

//organize error returns: malloc, logic
int	action_shift(t_stack *stack, t_entry *entry, t_token_stack *tokens)
{
	t_token_node	*popped_token;
	t_node			*token;
	t_ast_node		*ast_node;

	if (get_debug())
		ft_putendl_fd("-shift-", 1);
	popped_token = pop_token(tokens);
	if (!popped_token)
		return (-2);

	//--create_ast_node
	ast_node = gc_alloc(sizeof(t_ast_node));
	if (!ast_node)
		return (-2);
	ast_node->type = popped_token->type;
	ast_node->token = popped_token;
	//--create_ast_node

	//--create_token and link ast_node
	token = init_node(popped_token->type);
	if (!token)
		return (-2);
	token->ast_node = ast_node;
	//--create_token and link ast_node
	push(stack, token);

	return (action_goto(stack, entry));
}

int	action_reduce(t_stack *stack, t_entry *entry, t_entry **table)
{
	int	non_terminal;
	int	current_state;
	t_node	*rule;
	t_entry	*goto_entry;

	if (get_debug())
		ft_putendl_fd("-reduce-", 1);

	//--ast_node type fetch
	non_terminal = get_non_terminal(entry->go_to);
	if (non_terminal == -1)
		return (-2);
	//--ast_node type fetch

	//--ast_node children array alloc
	t_ast_node	**children;
	children = NULL;
	if (entry->reduce > 0)
	{
		children = gc_alloc(sizeof(t_ast_node *) * entry->reduce);
		if (!children)
			return (-2);
	}
	//--ast_node children array alloc
	
	//--ast_node children fill
	int	i;
	t_node *state;
	t_node *symbol;

	i = entry->reduce;
	while(0 < i--)
	{
		state = pop(stack);
		if (!state)
			return (-2);
		symbol = pop(stack);
		if (!symbol)
			return (-2);
		if (symbol->ast_node)
			children[i] = symbol->ast_node;
		else
			children[i] = NULL;
		//free state and symbol?
	}
	//--ast_node children fill

	//current_state
	if (!stack->top)
		return (-2);
	current_state = stack->top->value; //fetch_top(stack);

	//--create_ast_node
	t_ast_node	*ast_node;
	ast_node = gc_alloc(sizeof(t_ast_node));
	if (!ast_node)
	{
		if (children)
			gc_free(children);
		return (-2);
	}
	//--create_ast_node
	ast_node->type = non_terminal;
	ast_node->children = children;
	ast_node->child_count = entry->reduce;
	
	//--push non-terminal to stack
	rule = init_node(non_terminal);
	if (!rule)
	{
		if (ast_node)
			gc_free(ast_node);
		if (children)
			gc_free(children);
		return (-2);
	}
	rule->ast_node = ast_node;
	push(stack, rule);
	//--push non-terminal to stack

	goto_entry = non_terminal_lookup(table, current_state, non_terminal);
	if (!goto_entry)
		return (-2); //goto entry not found

	return(action_goto(stack, goto_entry));
}

int	action_accept(void) //t_stack *stack
{
	if (get_debug())
	{
		ft_putendl_fd("-accept-", 1);
		ft_putendl_fd("parsing accepted", 1);
	}
	/*
	t_node	*state;
	t_node	*symbol;

	state = pop(stack);
	if (!state)
		return (-2);
	symbol = pop(stack);
	if (!symbol)
		return (-2);

	//--create_ast_node
	t_ast_node	*ast_node;
	ast_node = gc_alloc(sizeof(t_ast_node));
	if (!ast_node)
		return (-2);
	ast_node->type = ACCEP;
	ast_node->token = NULL;
	ast_node->child_count = 1;
	ast_node->children = gc_alloc(sizeof(t_ast_node *));
	if (!ast_node->children)
		return (-2);
	ast_node->children[0] = symbol->ast_node;
	//--create_ast_node

	t_node	*accept;
	accept = init_node(ast_node->type);
	if (!accept)
		return (-2);
	accept->ast_node = ast_node;
	push(stack, accept);
	*/
	return (1);
}

int	action_goto(t_stack *stack, t_entry *entry)
{
	t_node	*next_state;

	//ft_putendl_fd("-adding goto-", 1);
	next_state = init_node(entry->go_to);
	if (!next_state)
		return (-2); //error
	push(stack, next_state);
	return (-1);
}