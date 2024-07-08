/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 19:18:18 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/08 20:23:33 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_exit(char *args)
{
	int	exit_status;

	exit_status = get_exit_status();
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
