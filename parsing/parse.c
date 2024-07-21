/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 11:09:11 by baouragh          #+#    #+#             */
/*   Updated: 2024/07/18 17:26:21 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../includes/minishell.h"

t_redir	*do_red(t_token **tokens) // 
{
	t_redir *new;

	new = malloc(sizeof(t_redir));
	if(!new)
		return(NULL);
	gc_add(g_minishell, new);
	ft_bzero(new, sizeof(t_redir));
	new->type = (*tokens)->type;
	if(new->type == L_REDIR)
		new->mode = O_RDONLY;
	else if(new->type == R_REDIR)
		new->mode = O_CREAT | O_RDWR;
	else if(new->type == RR_REDIR)
		new->mode = O_CREAT | O_RDWR | O_APPEND;
	(*tokens) = (*tokens)->next;
	if (ft_strchr((*tokens)->value, '$'))
			here_doc_expanding(&(*tokens)->value);
	new->file = (*tokens)->value;
	return (new);
}

t_node	*parse_pipe(t_token **tokens);
t_node	*parse_or(t_token **tokens);
t_node	*parse_and(t_token **tokens);
t_node	*parse_block(t_token **tokens);

t_node *parse_cmd(t_token **tokens) // (1 && 2) > 1 && 3
{
	t_list *cmd_list;
	t_list *red_list;
	t_list *new;
	t_redir *red;
	t_node *block;

	// () > 1
	block = NULL;
	cmd_list = NULL;
	red_list = NULL;
	while(*tokens && ((*tokens)->type != END && (*tokens)->type != PIPE && (*tokens)->type != OR && (*tokens)->type != AND && (*tokens)->type != R_PAREN))
	{
		if((*tokens)->type >= 4 && (*tokens)->type <= 7)
		{
			red = do_red(tokens);
			new = ft_lstnew(red);
			if (!new)
				return(NULL);
			gc_add(g_minishell, new);
			ft_lstadd_back(&red_list, new);
		}
		else if((*tokens)->type == WORD)
		{
			new = ft_lstnew((*tokens)->value);
			if (!new)
				return(NULL);
			gc_add(g_minishell, new);
			ft_lstadd_back(&cmd_list,new);
		}
		else if((*tokens)->type == L_PAREN)
		{
			(*tokens) = (*tokens)->next;
			block = parse_block(tokens); // 1 && 2
			return(block);
		}
		(*tokens) = (*tokens)->next;
	}
	// if(block && !red_list)
	// 	return (block);
	if(!red_list)
		return(string_node_new(cmd_list));
	else
	{
		red->cmd = cmd_list;
		return(redir_node_new(red_list));
	}
}

t_node	*parse_pipe(t_token **tokens) // (ls && ps) // $sdads | ls
{
	t_node *left;
	t_type type;

	left = NULL;
	// if((*tokens)->type == L_PAREN)
	//		 left = parse_block(tokens);
	if(*tokens && ((*tokens)->type == WORD || ((*tokens)->type >= 4 && (*tokens)->type <= 7) || (*tokens)->type == L_PAREN))
		left = parse_cmd(tokens);
	if(*tokens && ((*tokens)->type == PIPE))
	{
		type = (*tokens)->type;
		(*tokens) = (*tokens)->next;
		return (pair_node_new(left, parse_pipe(tokens), type));
	}
	// else if(!left)
	//	 return(error_node_new("invalid pair node from pipe func\n"));
	else
		return (left);
}

t_node	*parse_or(t_token **tokens) // (ls && ps) || ls && p
{
	t_node *left;
	t_type type;

	left = parse_pipe(tokens);
	if(*tokens && (*tokens)->type == OR)
	{
		type = (*tokens)->type;
		(*tokens) = (*tokens)->next;
		return (pair_node_new(left, parse_or(tokens), type));
	}
	else
		return(left);
}

t_node	*parse_and(t_token **tokens)
{
	t_node *left;
	t_type type;

	left = parse_or(tokens);
	if(*tokens && (*tokens)->type == AND)
	{
		type = (*tokens)->type;
		(*tokens) = (*tokens)->next;
		return (pair_node_new(left, parse_and(tokens), type));
	}
	else
		return(left);
}

t_node *find_last_left(t_node *node)
{
	t_node *left;
	while(node) // 1->2->3->4->leaf
	{
		if(node->type != PAIR_NODE)
			return (node);
		left = node;
		node = node->data.pair.left;
	}
	return(left);
}

void set_left_redir(t_node **left, t_node **right)
{
	t_list *last;
	t_redir *red;

	last = ft_lstlast((*right)->data.redir);
	red = (t_redir *)last->content;
	red->node = *left;
}
t_node	*parse_block(t_token **tokens) // (ls -a) > 1 || (ls -la | cat) > 1
{
	t_node *left;
	t_node *right;
	t_node *tmp;

	left = parse_and(tokens);
	if(tokens && *tokens && (*tokens)->type == R_PAREN)
	{
		(*tokens) = (*tokens)->next;
		
		right = parse_block(tokens);
		if(right)
		{
			if(right->type == PAIR_NODE)
			{
				if(!right->data.pair.left)
					right->data.pair.left = left;
				else
				{
					tmp = find_last_left(right);
					if(tmp->type == PAIR_NODE)
						tmp->data.pair.left = left;
					else if(tmp->type == REDIR_NODE)
						set_left_redir(&left, &tmp);
				}
			}
			else
				set_left_redir(&left, &right);
			return(right);
		}
	}
	return(left);
}

void set_null_as_true(t_node **res)
{
	t_list *true_c;
	char *str;
	char *path;

	path = get_fullpath("true", env_to_envp(g_minishell->our_env));
	gc_add(g_minishell, path);
	if(get_env_var(g_minishell->our_env, "PATH") && path)
	{
		str = ft_strdup("true");
		gc_add(g_minishell, str);
		true_c = ft_lstnew(str);
		gc_add(g_minishell, true_c);
		if(*res && (*res)->type == PAIR_NODE)
		{
			if(!(*res)->data.pair.left)
				(*res)->data.pair.left = string_node_new(true_c);
			else if((*res)->data.pair.left->type == PAIR_NODE)
				set_null_as_true(&(*res)->data.pair.left);
			if(!(*res)->data.pair.right)
				(*res)->data.pair.right = string_node_new(true_c);
			else if((*res)->data.pair.right->type == PAIR_NODE)
				set_null_as_true(&(*res)->data.pair.right);
		}
	}
}
t_node	*parsing(void) // (ls -a) > 1
{
	t_node	*res;

	expander();
	res = parse_block(&g_minishell->tokens);
	if (!res)
		gc_free_all(g_minishell);
	set_null_as_true(&res);
	return(res);
}
