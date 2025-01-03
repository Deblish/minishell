/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 17:23:15 by aapadill          #+#    #+#             */
/*   Updated: 2024/11/27 17:23:16 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

//aka init_token_node
t_token_node	*create_token(t_token type, char *value)
{
	t_token_node	*new_node;

	new_node = gc_alloc(sizeof(t_token_node));
	if (!new_node)
		return (NULL);
	new_node -> type = type;
	new_node -> value = value;
	return (new_node);
}

void	print_token_stack(t_token_stack *stack, char *name)
{
	t_token_node	*node;

	node = stack -> top;
	ft_putendl_fd(name, 1);
	printf("The top element is -->%s<--\n", node -> value);
	while (node)
	{
		ft_putstr_fd("Token type->", 1);
		ft_putnbr_fd(node->type, 1);
		ft_putstr_fd("\tValue->", 1);
		ft_putstr_fd(node->value, 1);
		ft_putstr_fd("\n", 1);
		node = node -> next;
	}
}
