/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karpatel <karpatel@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:27:45 by karpatel          #+#    #+#             */
/*   Updated: 2024/12/17 15:27:46 by karpatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_node_unset(char *key)
{
	t_env	*env;
	t_env	*prev;

	env = g_env;
	prev = NULL;
	while (env)
	{
		if (!ft_strcmp(key, env -> key))
			break ;
		prev = env;
		env = env -> next;
	}
	if (!env)
		return ;
	if (prev != NULL)
		prev -> next = env -> next;
	else
		prev->next = env->next;
	env -> next = NULL;
	free(env -> key);
	free(env -> value);
	free(env);
}

void	unset_var(char **av)
{
	int		i;

	i = 1;
	while (av[i])
	{
		free_node_unset(av[i]);
		i++;
	}
	update_envp();
}
//unset head properly

int	ft_unset(char **av)
{
	if (av[1] && av[1][0] == '-')
	{
		printf("minishell: unset: options aren't supported\n");
		return (1);
	}
	if (get_env(av[1]) == NULL)
		return (0);
	free_node_unset(av[1]);
	return (0);
}
