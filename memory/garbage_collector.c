/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 09:22:45 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/12 19:18:18 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	small_add_back(t_gc **gc, t_gc *new_node)
{
	t_gc	*curr;

	if (*gc)
	{
		curr = *gc;
		while (curr->next)
			curr = curr->next;
		curr->next = new_node;
	}
	else
		*gc = new_node;
}

void	gc_add(t_minishell *mini, void *ptr)
{
	t_gc	*new_node;

	new_node = malloc(sizeof(t_gc));
	if (!new_node)
	{
		perror("malloc failed !");
		exit(2);
	}
	new_node->ptr = ptr;
	new_node->next = NULL;
	small_add_back(&mini->gc, new_node);
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
