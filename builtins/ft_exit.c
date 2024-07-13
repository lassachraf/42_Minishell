/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 19:18:18 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/13 14:34:54 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_exit(char *args)
{
	int	exit_status;

	exit_status = g_minishell->exit_s;
	if (args)
	{
		// don't have to 
		exit_status = ft_atoi(args);
	}
	ft_putstr_fd("exit\n", 1);
	clear_env(g_minishell->our_env);
	gc_free_all(g_minishell);
	free(g_minishell);
	exit(exit_status);
}
