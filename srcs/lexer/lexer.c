/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 12:46:07 by aapadill          #+#    #+#             */
/*   Updated: 2024/11/30 16:57:06 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Hardcored lexer just to be able to test the LR parsing algo,
 * we obviously need a proper lexer that takes the original raw string
 * and returns a stack of input tokens, remember,
 * we are reading from left to right, so we fill the stack from right to left
 *
 * test case:
 * ls > outfile EOF
 * WORD RED_TO WORD EOF
 *
 * ls > > ls
 * WORD RED_TO RED_TO WORD EOF
 *
 * ls | cat EOF
 * WORD PIPE WORD EOF
 *
 * btw, should prototype be?
 * int lexer(char *str, t_stack *input_tokens);
 *
 * or
 *
 * t_stack *lexer(char *str);
 */

//ls < ls ls | cat | ls > outfile

t_token_stack	*reverse_stack(t_token_stack *stack)
{
	t_token_node	*curr;
	t_token_node	*curr_temp;
	t_token_node	*previous;

	previous = NULL;
	curr = stack -> top;
	while (curr)
	{
		curr_temp = curr->next;
		curr->next = previous;
		previous = curr;
		curr = curr_temp;
	}
	stack->top = previous;
	return (stack);
}

void	init_iterators(t_iterators *it, char *input)
{
	it->cur = 0;
	it->start = 0;
	it->len = ft_strlen(input);
	it->quote_char = ' ';
	it->input = input;
}

int	lexer_loop(t_iterators *it, t_token_stack *rev_tokens)
{
	if (it->input[it->cur] == ' ')
	{
		handle_space(it);
		return (1);
	}
	if (is_token(it->input[it->cur], it->input[it->cur + 1]))
	{
		if (tokenize_pipe(it, rev_tokens))
			return (1);
		else if (tokenize_input(it, rev_tokens))
			return (1);
		else if (tokenize_output(it, rev_tokens))
			return (1);
	}
	else
	{
		if (tokenize_words(it, rev_tokens))
			return (1);
	}
	return (0);
}

t_token_stack	*lexer(char *input)
{
	t_iterators		it;
	t_token_stack	*rev_tokens;
	t_token_stack	*result;
	t_token_node	*eof;

	init_iterators(&it, input);
	rev_tokens = gc_alloc(sizeof(t_token_stack));
	while (it.cur < it.len)
	{
		if (lexer_loop(&it, rev_tokens))
			continue ;
	}
	eof = create_token(END, gc_strdup("EOF"));
	push_token(rev_tokens, eof);
	result = reverse_stack(rev_tokens);
	if (get_debug())
		print_token_stack(rev_tokens, "TOKEN STACK : AFTER");
	return (result);
}
