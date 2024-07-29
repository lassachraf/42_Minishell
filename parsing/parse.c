/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 11:09:11 by baouragh          #+#    #+#             */
/*   Updated: 2024/07/28 09:03:30 by baouragh         ###   ########.fr       */
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

t_node	*parse_pipe(t_token **tokens);
t_node	*parse_or(t_token **tokens);
t_node	*parse_and(t_token **tokens);
t_node	*parse_block(t_token **tokens);

t_node	*parse_cmd(t_token **tokens)
{
	t_list	*cmd_list;
	t_list	*red_list;
	t_list	*new;
	t_redir	*red;
	t_node	*block;

	block = NULL;
	cmd_list = NULL;
	red_list = NULL;
	while (*tokens && ((*tokens)->type != END && (*tokens)->type != PIPE
			&& (*tokens)->type != OR && (*tokens)->type != AND
			&& (*tokens)->type != R_PAREN))
	{
		if ((*tokens)->type >= 4 && (*tokens)->type <= 7)
		{
			red = do_red(tokens);
			new = ft_lstnew(red);
			if (!new)
				return (NULL);
			gc_add(g_minishell, new);
			ft_lstadd_back(&red_list, new);
		}
		else if ((*tokens)->type == WORD)
		{
			new = ft_lstnew((*tokens)->value);
			if (!new)
				return (NULL);
			gc_add(g_minishell, new);
			ft_lstadd_back(&cmd_list, new);
		}
		else if ((*tokens)->type == L_PAREN)
		{
			(*tokens) = (*tokens)->next;
			block = parse_block(tokens); // 1 && 2
			return (block);
		}
		(*tokens) = (*tokens)->next;
	}
	if (!red_list)
		return (string_node_new(cmd_list));
	else
	{
		red->cmd = cmd_list;
		return (redir_node_new(red_list));
	}
}

t_node	*parse_pipe(t_token **tokens)
{
	t_node	*left;
	t_type	type;

	left = NULL;
	if (*tokens && ((*tokens)->type == WORD || ((*tokens)->type >= 4
				&& (*tokens)->type <= 7) || (*tokens)->type == L_PAREN))
		left = parse_cmd(tokens);
	if (*tokens && ((*tokens)->type == PIPE))
	{
		type = (*tokens)->type;
		(*tokens) = (*tokens)->next;
		return (pair_node_new(left, parse_pipe(tokens), type));
	}
	else
		return (left);
}

t_node	*parse_or(t_token **tokens)
{
	t_node	*left;
	t_type	type;

	left = parse_pipe(tokens);
	if (*tokens && (*tokens)->type == OR)
	{
		type = (*tokens)->type;
		(*tokens) = (*tokens)->next;
		return (pair_node_new(left, parse_or(tokens), type));
	}
	else
		return (left);
}

t_node	*parse_and(t_token **tokens)
{
	t_node	*left;
	t_type	type;

	left = parse_or(tokens);
	if (*tokens && (*tokens)->type == AND)
	{
		type = (*tokens)->type;
		(*tokens) = (*tokens)->next;
		return (pair_node_new(left, parse_and(tokens), type));
	}
	else
		return (left);
}

t_node	*find_last_left(t_node *node)
{
	t_node	*left;

	while (node)
	{
		if (node->type != PAIR_NODE)
			return (node);
		left = node;
		node = node->data.pair.left;
	}
	return (left);
}

void	set_left_redir(t_node **left, t_node **right)
{
	t_list	*last;
	t_redir	*red;

	last = ft_lstlast((*right)->data.redir);
	red = (t_redir *)last->content;
	red->node = *left;
}

void	adjust_right(t_node **right, t_node **left)
{
	t_node	*tmp;

	if (!right || !*right)
		return ;
	tmp = NULL;
	if ((*right)->type == PAIR_NODE)
	{
		if (!(*right)->data.pair.left)
			(*right)->data.pair.left = *left;
		else
		{
			tmp = find_last_left(*right);
			if (tmp->type == PAIR_NODE)
				tmp->data.pair.left = *left;
			else if (tmp->type == REDIR_NODE)
				set_left_redir(&*left, &tmp);
		}
	}
	else
		set_left_redir(&*left, &*right);
}

t_node	*parse_block(t_token **tokens)
{
	t_node	*left;
	t_node	*right;

	left = parse_and(tokens);
	if (tokens && *tokens && (*tokens)->type == R_PAREN)
	{
		(*tokens) = (*tokens)->next;
		right = parse_and(tokens);
		if (right)
		{
			adjust_right(&right, &left);
			return (right);
		}
	}
	return (left);
}

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

void	join_words(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		if (tokens->type == WORD && tokens->next_space == 0 && tokens->next
			&& tokens->next->type == WORD)
		{
			tmp = tokens;
			tokens = tokens->next;
			tokens->value = ft_strjoin(tmp->value, tokens->value);
			gc_add(g_minishell, tokens->value);
			remove_token(&tokens, tmp);
		}
		else
			tokens = tokens->next;
	}
}

t_node	*parsing(void)
{
	t_node	*res;

	expanding();
	remove_quotes(&g_minishell->tokens);
	join_words(g_minishell->tokens);
	res = parse_block(&g_minishell->tokens);
	if (!res)
		gc_free_all(g_minishell);
	return (res);
}
