/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 16:28:54 by aapadill          #+#    #+#             */
/*   Updated: 2024/11/28 19:12:26 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_node	*init_node(int value)
{
	t_node	*new_node;

	new_node = gc_alloc(sizeof(t_node));
	if (!new_node)
		return (NULL);
	new_node->value = value;
	//new_node->token = NULL;
	//new_node->next = NULL;
	return (new_node);
}

t_stack	*init_stack(void)
{
	t_stack	*new_stack;

	new_stack = gc_alloc(sizeof(t_stack));
	if (!new_stack)
		return (NULL);
	//new_stack->top = NULL;
	//new_stack->size = 0;
	return (new_stack);
}

void	print_stack(t_stack *stack, char *name)
{
	t_node *node;

	node = stack->top;
	ft_putstr_fd("---", 1);
	ft_putendl_fd(name, 1);
	while (node)
	{
		ft_putnbr_fd(node->value, 1);
		/*
		if (node->token)
		{
			ft_putstr_fd(" (", 1);
			ft_putstr_fd(node->token->value, 1);
			ft_putstr_fd(")", 1);
		}
		*/
		ft_putchar_fd('\n', 1);
		node = node->next;
	}
}

void	print_tokens(t_token_stack *tokens, char *name)
{
	t_token_node *node;

	node = tokens->top;
	ft_putstr_fd("---", 1);
	ft_putendl_fd(name, 1);
	while (node)
	{
		ft_putnbr_fd(node->type, 1);
		ft_putchar_fd('(', 1);
		ft_putstr_fd(node->value, 1);
		ft_putchar_fd(')', 1);
		ft_putchar_fd('\n', 1);
		node = node->next;
	}
}

/*
int	free_stack(t_stack *stack, int send_error)
{
	while (stack->size)
		free(pop(stack));
	free(stack);
	if (send_error)
		write(2, "Error\n", 6);
	return (0);
}
*/
