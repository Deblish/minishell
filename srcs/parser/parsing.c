/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 21:37:13 by aapadill          #+#    #+#             */
/*   Updated: 2024/11/30 15:05:48 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"
#include "execution.h"

t_entry *non_terminal_lookup(t_entry **table, int state, int non_terminal)
{
	int	i;

	i = 0;
	while (table[i])
	{
		if (table[i]->state == state && 
			table[i]->token_type == non_terminal && 
			table[i]->action == GOTO)
			return (table[i]);
		i++;
	}
	return (NULL);
}

t_entry *actual_lookup(t_entry **table, int state, int token)
{
	int	i;

	i = 0;
	if (!table)
		return (NULL);
	while (table[i])
	{
		if (table[i]->state == state && table[i]->token_type == token)
			return (table[i]);
		i++;
	}
	return (NULL);
}

t_entry	*table_lookup(t_stack *stack, t_token_stack *tokens, t_entry **table)
{
	int	state;
	int	token;
	t_entry	*entry;

	state = -2;
	if (stack->top)
		state = stack->top->value; //fetch_top(stack);
	token = -2;
	if (tokens->top)
		token = tokens->top->type; //fetch_top(in_stack);
	entry = NULL;
	entry = actual_lookup(table, state, token);
	if (!entry)
		entry = actual_lookup(table, state, DEFAULT);
	return (entry);
}

int	parsing_main(t_token_stack *tokens, t_entry **table)
{
	int		ret;
	t_stack	*stack;
	t_node	*node;
	t_entry	*entry;

	entry = NULL;
	ret = 0;
	stack = init_stack();
	if (!init_stack())
		return (-1);
	node = init_node(0);
	if (!init_node(0))
		return (-1);
	push(stack, node);
	ret = -1;
	while (ret == -1)
	{
		if (get_debug())
		{
			ft_putendl_fd("----------------", 1);
			if (stack->top)
				print_stack(stack, "stack");
			if (tokens->top)
				print_tokens(tokens, "tokens");
		}
		entry = table_lookup(stack, tokens, table);
		if (!entry)
		{
			ret = 2;
			break ;
		}
		else if (entry->action == ACCEPT)
			ret = action_accept(); //stack
		else if (entry->action == SHIFT)
			ret = action_shift(stack, entry, tokens);
		else if (entry->action == REDUCE)
			ret = action_reduce(stack, entry, table);
		else
			ret = 0;
	}
	if (get_debug())
	{
		ft_putendl_fd("-------ast--------", 1);
		traverse_ast(get_ast_root(stack), 0);
	}
	execute_ast(get_ast_root(stack));
	/*ft_putendl_fd("----------------leftovers", 1);
	print_stack(stack, "stack");
	ft_putendl_fd("", 1);
	print_tokens(tokens, "tokens");*/
	return (ret);
}
