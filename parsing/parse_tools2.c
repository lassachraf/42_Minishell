/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tools2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 11:55:27 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/03 19:15:08 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	execlude_null(t_list **lst)
{
	t_list	*res;
	t_list	*new;

	if (!lst || !*lst)
		return ;
	res = NULL;
	while (*lst)
	{
		if ((*lst)->content)
		{
			new = ft_lstnew((*lst)->content);
			gc_add(g_minishell, new);
			ft_lstadd_back(&res, new);
		}
		(*lst) = (*lst)->next;
	}
	(*lst) = res;
}

void	remove_null(t_node **res)
{
	t_node	*left;
	t_node	*right;

	if (!res || !*res)
		return ;
	if ((*res)->type == PAIR_NODE)
	{
		left = (*res)->data.pair.left;
		right = (*res)->data.pair.right;
		if (left && left->type == PAIR_NODE)
			remove_null(&left);
		if (right && right->type == PAIR_NODE)
			remove_null(&right);
	}
	else if ((*res)->type == STRING_NODE)
		execlude_null(&(*res)->data.cmd);
}

void	join_words(t_token **tokens)
{
	t_token	*curr;
	t_token	*temp;

	temp = *tokens;
	while (temp)
	{
		if (temp->type == WORD && temp->next_space == 0 && temp->next
			&& temp->next->type == WORD)
		{
			curr = temp;
			temp = temp->next;
			temp->value = ft_strjoin(curr->value, temp->value);
			temp->wd_expand = 0;
			if (ft_strchr(curr->value, '*') || ft_strchr(temp->value, '*'))
				temp->wd_expand = curr->wd_expand;
			if (ft_strchr(curr->value, '$'))
				temp->quoted = curr->quoted;
			gc_add(g_minishell, temp->value);
			remove_token(tokens, curr);
		}
		else
			temp = temp->next;
	}
}

bool	is_parse_separator(t_token **tokens)
{
	if (*tokens && ((*tokens)->type != END && (*tokens)->type != PIPE
			&& (*tokens)->type != OR && (*tokens)->type != AND
			&& (*tokens)->type != R_PAREN))
		return (1);
	return (0);
}

t_node	*parse_cmd(t_token **tokens)
{
	t_list	*cmd_list;
	t_list	*red_list;
	t_redir	*red;

	cmd_list = NULL;
	red_list = NULL;
	while (is_parse_separator(tokens))
	{
		if ((*tokens)->type >= 4 && (*tokens)->type <= 7)
			add_redir(tokens, &red_list);
		else if ((*tokens)->type == WORD)
			add_cmd(tokens, &cmd_list);
		else if ((*tokens)->type == L_PAREN)
			return (parse_inside(tokens));
		(*tokens) = (*tokens)->next;
	}
	if (!red_list)
		return (string_node_new(cmd_list));
	else
	{
		red = ft_lstlast(red_list)->content;
		red->cmd = cmd_list;
		return (redir_node_new(red_list));
	}
}
