/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   info.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 10:27:05 by aapadill          #+#    #+#             */
/*   Updated: 2024/12/16 10:27:31 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_info	*get_info(void)
{
	static t_info	info;

	return (&info);
}

void	set_debug(bool	debug)
{
	t_info	*info;

	info = get_info();
	info->debug = debug;
}

bool	get_debug(void)
{
	t_info	*info;

	info = get_info();
	return (info->debug);
}