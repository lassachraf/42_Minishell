/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 14:02:16 by baouragh          #+#    #+#             */
/*   Updated: 2024/06/13 15:58:38 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_node	*redir_node_new(t_list *red_list)
{
	t_node *new;
	new = malloc(sizeof(t_node));
	if(!new)
			return(NULL);
	new->type = REDIR_NODE;
	new->data.redir = red_list;
	return (new);
}

t_node	*char_node_new(char c)
{
	t_node *new;
	new = malloc(sizeof(t_node));
	if(!new)
			return(NULL);
	new->type = CHAR_NODE;
	new->data.char_value = c;
	return (new);
}
t_node	*string_node_new(t_list *list)
{
	t_node *new;
	new = malloc(sizeof(t_node));
	if(!new)
			return(NULL);
	new->type = STRING_NODE;
	new->data.cmd = list;
	return (new);
}
t_node	*pair_node_new(t_node *left, t_node *right, t_type type)
{
	t_node *new;
	new = malloc(sizeof(t_node));
	if(!new)
			return(NULL);
	new->type = PAIR_NODE;
	new->data.pair.left = left;
	new->data.pair.right = right;
	new->data.pair.type = type;
	return (new);
}
t_node	*error_node_new(const char *msg)
{
	t_node *new;
	new = malloc(sizeof(t_node));
	if(!new)
			return(NULL);
	new->type = ERROR_NODE;
	new->data.error = msg;
	return (new);
}
