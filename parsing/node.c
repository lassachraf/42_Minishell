/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 14:02:16 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/09 12:35:46 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_node	*redir_node_new(t_list *red_list)
{
	t_node	*new;

	new = malloc(sizeof(t_node));
	if (!new)
		return (NULL);
	gc_add(g_minishell, new);
	new->type = REDIR_NODE;
	new->data.redir = red_list;
	if (new->data.redir)
		new->data.redir->is_block = 0;
	return (new);
}

t_node	*string_node_new(t_list *list)
{
	t_node	*new;

	new = malloc(sizeof(t_node));
	if (!new)
		return (NULL);
	gc_add(g_minishell, new);
	new->type = STRING_NODE;
	new->data.cmd = list;
	if (new->data.cmd)
		new->data.cmd->is_block = 0;
	return (new);
}

t_node	*pair_node_new(t_node *left, t_node *right, t_type type)
{
	t_node	*new;

	new = malloc(sizeof(t_node));
	if (!new)
		return (NULL);
	gc_add(g_minishell, new);
	new->type = PAIR_NODE;
	new->data.pair.left = left;
	new->data.pair.right = right;
	new->data.pair.type = type;
	new->data.pair.is_block = 0;
	return (new);
}

int	remove_dollar(t_token **temp)
{
	t_token	*tmp;

	if ((*temp)->type == WORD && (*temp)->value && (*temp)->next->value
		&& !ft_strcmp((*temp)->value, "$")
		&& !ft_strcmp((*temp)->next->value, ""))
	{
		tmp = (*temp)->next;
		remove_token(&g_minishell->tokens, *temp);
		(*temp) = tmp;
		return (1);
	}
	else if ((*temp)->value && !ft_strcmp((*temp)->value, "$")
		&& !(*temp)->quoted && (*temp)->next->value
		&& (*temp)->next->quoted > 0)
	{
		tmp = (*temp)->next;
		remove_token(&g_minishell->tokens, *temp);
		(*temp) = tmp;
		return (1);
	}
	return (0);
}

void	join_for_asterisk(t_token **tokens)
{
	t_token	*temp;

	temp = *tokens;
	while (temp)
	{
		if (remove_dollar(&temp))
			continue ;
		if (temp->type == WORD && temp->value && ft_strchr(temp->value, '*')
			&& temp->next_space == 0)
		{
			temp->next->wd_expand = temp->wd_expand;
			temp = temp->next;
		}
		else if (temp->type == WORD && temp->next && temp->next_space == 0
			&& temp->next->value && ft_strchr(temp->next->value, '*'))
		{
			temp->wd_expand = temp->next->wd_expand;
			temp = temp->next;
		}
		else
			temp = temp->next;
	}
}
