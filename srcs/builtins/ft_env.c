/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karpatel <karpatel@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:27:15 by karpatel          #+#    #+#             */
/*   Updated: 2024/12/17 15:27:17 by karpatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(char **av)
{
	if (ft_strcmp("env", av[0]))
		return (0);
	if (av[0] && av[1])
	{
		printf("minishell: env: Arguments and options aren't supported\n");
		return (1);
	}
	env_to_envp_print();
	return (0);
}
