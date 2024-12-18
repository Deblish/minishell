/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 17:22:39 by aapadill          #+#    #+#             */
/*   Updated: 2024/11/27 17:22:41 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void	handle_space(t_iterators *it)
{
	while (it->input[it->cur] != '\0' && it->input[it->cur] == ' ')
	{
		it->cur++;
	}
}

bool	tokenize_pipe(t_iterators *it, t_token_stack *stack)
{
	t_token_node	*new;

	if (it->input[it->cur] == '|')
	{
		new = create_token(PIPE, gc_strdup("|"));
		if (!new)
			gc_free_all(); //ok this just free so we need a gc_free_all_exit
		push_token(stack, new);
		it->cur++;
		return (true);
	}
	return (false);
}

bool	tokenize_output(t_iterators *it, t_token_stack *stack)
{
	if (it->input[it->cur] == '>')
	{
		if (it->input[it->cur + 1] && it->input[it->cur + 1] == '>')
		{
			push_token(stack, create_token(DGREAT, gc_strdup(">>")));
			it->cur += 2;
		}
		else
		{
			push_token(stack, create_token(RED_TO, gc_strdup(">")));
			it->cur++;
		}
		return (true);
	}
	return (false);
}

bool	tokenize_input(t_iterators *it, t_token_stack *stack)
{
	if (it->input[it->cur] == '<')
	{
		if (it->cur + 1 < it->len && it->input[it->cur + 1] == '<')
		{
			push_token(stack, create_token(DLESS, gc_strdup("<<")));
			it->cur += 2;
		}
		else
		{
			push_token(stack, create_token(RED_FO, gc_strdup("<")));
			it -> cur ++;
		}
		return (true);
	}
	return (false);
}

bool	tokenize_words(t_iterators *it, t_token_stack *stack)
{
	t_token_node	*node;
	char			*word;
	char			quote;

	it->start = it->cur;
	while (it->cur < it->len && it->input[it->cur] != ' '
		&& !is_token(it->input[it->cur], it->input[it->cur + 1]))
	{
		if (it->input[it->cur] == '\'' || it->input[it->cur] == '"')
		{
			quote = it->input[it->cur++];
			while (it->cur < it->len && it->input[it->cur] != quote)
				it->cur++;
			if (it->cur < it->len)
				it->cur++;
		}
		else
			it->cur++;
	}
	word = gc_strndup(&it->input[it->start], it->cur - it->start);
	node = create_token(WORD, word);
	/*
	if (quote == '\"')
		node->state = DOUBLE_QUOTES;
	else if (quote == '\'')
		node->state = SINGLE_QUOTES;
	*/
	push_token(stack, node);
	return (true);
}