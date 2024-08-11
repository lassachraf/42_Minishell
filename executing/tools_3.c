/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 18:37:43 by alassiqu          #+#    #+#             */
/*   Updated: 2024/08/10 18:38:21 by alassiqu         ###   ########.fr       */
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
