/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 09:20:20 by alassiqu          #+#    #+#             */
/*   Updated: 2024/06/14 09:27:22 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_malloc(t_minishell *mini, size_t size)
{
	char	*memory;

	memory = malloc(size);
	if (!memory)
		return (perror("malloc failed!"), NULL);
	gc_add(mini, memory);
	return (memory);
}
