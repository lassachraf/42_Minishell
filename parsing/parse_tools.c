/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 11:54:28 by baouragh          #+#    #+#             */
/*   Updated: 2024/08/05 09:11:26 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_redir	*do_red(t_token **tokens)
{
	t_redir	*new;

	new = malloc(sizeof(t_redir));
	if (!new)
		return (NULL);
	gc_add(g_minishell, new);
	ft_bzero(new, sizeof(t_redir));
	new->type = (*tokens)->type;
	if (new->type == L_REDIR)
		new->mode = O_RDONLY;
	else if (new->type == R_REDIR)
		new->mode = O_CREAT | O_RDWR;
	else if (new->type == RR_REDIR)
		new->mode = O_CREAT | O_RDWR | O_APPEND;
	(*tokens) = (*tokens)->next;
	new->file = (*tokens)->value;
	new->hd_expand = (*tokens)->hd_expand;
	return (new);
}

void	set_left_redir(t_node **left, t_node **right)
{
	t_list	*last;
	t_redir	*red;

	last = ft_lstlast((*right)->data.redir);
	red = (t_redir *)last->content;
	red->node = *left;
}

void	add_redir(t_token **tokens, t_list **red_list)
{
	t_list	*new;
	t_redir	*red;

	red = do_red(tokens);
	new = ft_lstnew(red);
	if (!new)
		return ;
	gc_add(g_minishell, new);
	ft_lstadd_back(red_list, new);
}

void	add_cmd(t_token **tokens, t_list **cmd_list)
{
	t_list	*new;

	new = ft_lstnew((*tokens)->value);
	if (!new)
		return ;
	new->wd_expand = (*tokens)->wd_expand;
	gc_add(g_minishell, new);
	ft_lstadd_back(cmd_list, new);
}

t_node	*parse_inside(t_token **tokens)
{
	t_node	*block;
	t_node	*redir;

	block = NULL;
	(*tokens) = (*tokens)->next;
	block = parse_and(tokens);
	if (block && block->type == PAIR_NODE)
		block->data.pair.is_block = 1;
	else if (block && block->type == STRING_NODE)
		block->data.cmd->is_block = 1;
	else if (block && block->type == REDIR_NODE)
		block->data.redir->is_block = 1;
	(*tokens) = (*tokens)->next;
	while ((*tokens)->type == WHITESPACE)
		(*tokens) = (*tokens)->next;
	if ((*tokens)->type >= 4 && (*tokens)->type <= 7)
	{
		redir = parse_cmd(tokens);
		if (block && redir->type == REDIR_NODE)
			return (set_left_redir(&block, &redir), redir);
	}
	return (block);
}
