/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:13:03 by baouragh          #+#    #+#             */
/*   Updated: 2024/07/17 09:55:57 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	do_here_docs(t_list *red_list, int doc_num)
{
	t_redir	*new;

	while (red_list)
	{
		new = red_list->content;
		if (new->type == LL_REDIR)
		{
			g_minishell->docs++;
			new->fd = here_doc(new->file, doc_num);
			if (new->fd < 0)
				return (0);
		}
		red_list = red_list->next;
	}
	return (1);
}

int	input_to_dup(t_list *red_list)
{
	t_redir	*new;
	int		fd;

	fd = -1;
	while (red_list)
	{
		new = red_list->content;
		if (new->type == L_REDIR || new->type == LL_REDIR)
			fd = new->fd;
		red_list = red_list->next;
	}
	return (fd);
}

int	output_to_dup(t_list *red_list)
{
	t_redir	*new;
	int		fd;

	fd = -1;
	while (red_list)
	{
		new = red_list->content;
		if (new->type == R_REDIR || new->type == RR_REDIR)
			fd = new->fd;
		red_list = red_list->next;
	}
	return (fd);
}

void	run_doc_cmd(t_list *red_list)
{
	t_list	*last;
	t_redir	*new;

	last = ft_lstlast(red_list);
	new = last->content;
	if (new->cmd)
		executer(string_node_new(new->cmd));
	else if (new->node)
		executer(new->node);
}

int	scan_and_set(t_node *node)
{
	if (!node)
		return (1);
	if (node->type == PAIR_NODE)
	{
		if (!scan_and_set(node->data.pair.left))
			return (0);
		if (!scan_and_set(node->data.pair.right))
			return (0);
	}
	else if (node->type == REDIR_NODE)
		return (execute_docs(node->data.redir));
	return (1);
}
