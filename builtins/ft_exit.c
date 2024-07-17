/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 19:18:18 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/17 10:35:21 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_exit(char *args)
{
	int	exit_status;

	exit_status = g_minishell->exit_s;
	if (args)
	{
		exit_status = ft_atoi(args);
	}
	ft_putstr_fd("exit\n", 1);
	clear_env(g_minishell->our_env);
	gc_free_all(g_minishell);
	free(g_minishell);
	exit(exit_status);
}
