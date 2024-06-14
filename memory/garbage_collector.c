/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 09:22:45 by alassiqu          #+#    #+#             */
/*   Updated: 2024/06/14 09:27:04 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdlib.h>

void	gc_add(t_minishell *mini, void *ptr)
{
	t_gc	*new_node;

	new_node = malloc(sizeof(t_gc));
	if (!new_node)
	{
		// Handle allocation failure
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	new_node->ptr = ptr;
	new_node->next = mini->gc;
	mini->gc = new_node;
}

void	gc_free_all(t_minishell *mini)
{
	t_gc	*current;
	t_gc	*next;

	current = mini->gc;
	while (current)
	{
		next = current->next;
		free(current->ptr);
		free(current);
		current = next;
	}
	mini->gc = NULL;
}
