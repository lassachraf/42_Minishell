/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 10:44:14 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/15 15:39:23 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	remove_token(t_token **head, t_token *token)
{
	if (!token->prev)
	{
		*head = (*head)->next;
		if (*head)
			(*head)->prev = NULL;
	}
	else
	{
		token->prev->next = token->next;
		if (token->next)
			token->next->prev = token->prev;
	}
}

void	handle_special_case(t_token **tokens, t_token **current)
{
	t_token	*tmp;

	tmp = (*current)->next;
	remove_token(tokens, (*current));
	(*current) = tmp;
	(*current)->value = ft_strdup("\0");
	gc_add(g_minishell, (*current)->value);
	(*current)->type = WORD;
	(*current) = (*current)->next;
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

void	remove_quotes(t_token **tokens)
{
	t_token	*current;
	t_token	*tmp;

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
			{
				handle_special_case(tokens, &current);
			}
			else
			{
				tmp = current->next;
				remove_token(tokens, current);
				current = tmp;
			}
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
			tmp = current->next;
			remove_token(tokens, current);
			current = tmp;
		}
		else
			current = current->next;
	}
}
