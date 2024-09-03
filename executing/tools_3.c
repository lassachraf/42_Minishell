/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 18:37:43 by alassiqu          #+#    #+#             */
/*   Updated: 2024/09/03 08:55:17 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_for_whitespaces(char *argv)
{
	int	i;

	i = 0;
	while (argv[i])
	{
		if (ft_isspace(argv[i]))
			return (1);
		i++;
	}
	return (0);
}

void	save_status_clean(void)
{
	int	e_s;

	e_s = g_minishell->exit_s;
	cleanup_minishell();
	exit(e_s);
}
