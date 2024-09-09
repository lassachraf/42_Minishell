/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 11:09:11 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/09 11:33:17 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_node	*parse_pipe(t_token **tokens)
{
	t_node	*left;
	t_node	*right;
	t_type	type;

	left = NULL;
	right = NULL;
	if (*tokens && ((*tokens)->type == WORD || ((*tokens)->type >= 4
				&& (*tokens)->type <= 7) || (*tokens)->type == L_PAREN))
		left = parse_cmd(tokens);
	if (*tokens && ((*tokens)->type == PIPE))
	{
		type = (*tokens)->type;
		(*tokens) = (*tokens)->next;
		right = parse_pipe(tokens);
		return (pair_node_new(left, right, type));
	}
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
	t_node	*right;
	t_type	type;

	left = parse_or(tokens);
	if (*tokens && (*tokens)->type == AND)
	{
		type = (*tokens)->type;
		(*tokens) = (*tokens)->next;
		right = parse_and(tokens);
		return (pair_node_new(left, right, type));
	}
	else
		return (left);
}

t_node	*parse_block(t_token **tokens)
{
	t_node	*left;

	left = parse_and(tokens);
	if (tokens && *tokens && (*tokens)->type == R_PAREN)
		(*tokens) = (*tokens)->next;
	return (left);
}

void	print_tokens(t_token *tokens)
{
	t_token	*token;

	token = tokens;
	while (token)
	{
		printf("value => '%s'\n", token->value);
		printf("type => '%u'\n", token->type);
		printf("quoted => '%u'\n", token->quoted);
		// printf("quoted => '%u'\n", token->);
		token = token->next;
	}
}

t_node	*parsing(void)
{
	t_node	*left;

	left = NULL;
	expanding();
	print_tokens(g_minishell->tokens);
	expand_dollar();
	join_for_asterisk(&g_minishell->tokens);
	print_tokens(g_minishell->tokens);
	join_words(&g_minishell->tokens);
	left = parse_block(&g_minishell->tokens);
	return (left);
}
