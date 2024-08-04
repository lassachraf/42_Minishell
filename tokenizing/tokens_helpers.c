/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 10:44:14 by alassiqu          #+#    #+#             */
/*   Updated: 2024/08/04 01:51:53 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_special_case(t_token **tokens, t_token **current)
{
	t_token	*tmp;

	if ((*current)->prev && (*current)->prev->type == WORD
		&& (*current)->prev->next_space == 0)
	{
		tmp = (*current)->next;
		if ((*current)->next_space == 1 && (*current)->prev)
			(*current)->prev->next_space = 1;
		remove_token(tokens, (*current));
		(*current) = tmp;
		return ;
	}
	else
	{
		tmp = (*current)->next;
		remove_token(tokens, (*current));
		(*current) = tmp;
		(*current)->value = ft_strdup("\0");
		gc_add(g_minishell, (*current)->value);
		(*current)->type = WORD;
		(*current) = (*current)->next;
	}
}

void	join_tokens(t_token **tokens, t_token **current)
{
	t_token	*tmp;
	char	*value;

	value = ft_strjoin((*current)->prev->value, (*current)->next->value);
	(*current)->prev->value = value;
	gc_add(g_minishell, value);
	tmp = (*current)->next->next;
	remove_token(tokens, (*current));
	remove_token(tokens, (*current)->next);
	(*current) = tmp;
	if ((*current)->type == D_QUOTE || (*current)->type == S_QUOTE)
	{
		tmp = (*current)->next;
		remove_token(tokens, (*current));
		(*current) = tmp;
	}
}

void	remove_it(t_token **tokens, t_token **current)
{
	t_token	*tmp;

	tmp = (*current)->next;
	if ((*current)->next_space == 1 && (*current)->prev)
		(*current)->prev->next_space = 1;
	remove_token(tokens, (*current));
	(*current) = tmp;
}

void	remove_quotes(t_token **tokens)
{
	t_token	*current;

	current = *tokens;
	while (current)
	{
		if (current->type == S_QUOTE || current->type == D_QUOTE)
		{
			if (current->prev && current->next && current->prev->type == WORD
				&& ft_strchr(current->prev->value, '=')
				&& current->next->type == WORD)
				join_tokens(tokens, &current);
			else if (current->next && special_case(current->prev, current,
					current->next))
				handle_special_case(tokens, &current);
			else
				remove_it(tokens, &current);
		}
		else
			current = current->next;
	}
}

void	remove_whitespaces(t_token **tokens)
{
	t_token	*current;
	t_token	*tmp;

	current = *tokens;
	while (current)
	{
		if (current->type == WHITESPACE)
		{
			if (current->prev)
				current->prev->next_space = 1;
			tmp = current->next;
			remove_token(tokens, current);
			current = tmp;
		}
		else if (current->type == END)
		{
			if (current->prev)
				current->prev->next_space = 1;
			current = current->next;
		}
		else
			current = current->next;
	}
}
