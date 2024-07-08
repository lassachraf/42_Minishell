/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 20:33:35 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/08 17:17:35 by alassiqu         ###   ########.fr       */
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

void	replace(t_token *current)
{
	current->value = ft_strdup("\0");
	gc_add(g_minishell, current->value);
	current->type = WORD;
}

int	special_case(t_type prev_type, t_type curr_type, t_type next_type)
{
	if ((curr_type == S_QUOTE || curr_type == D_QUOTE) && curr_type == next_type)
	{
		// "ls" "-la"
		if (prev_type == WORD)
			return (0);
		else
			return (1);
	}
	return (0);
}

void	remove_quotes(t_token **tokens)
{
	t_token	*current;
	t_token	*tmp;
	char	*value;

	current = *tokens;
	while (current)
	{
		// if (current->prev && current->next &&
		// 	special_case(current->prev->type, current->type, current->next->type))
		// {
		// 	tmp = current->next;
		// 	remove_token(tokens, current);
		// 	current = tmp;
		// 	replace(current);
		// 	current = current->next;
		// }
		if (current->type == S_QUOTE || current->type == D_QUOTE)
		{
			if (current->prev && current->next && current->prev->type == WORD
				&& current->next->type == WORD)
			{
				value = ft_strjoin(current->prev->value, current->next->value);
				current->prev->value = value;
				gc_add(g_minishell, value);
				tmp = current->next->next;
				remove_token(tokens, current);
				remove_token(tokens, current->next);
				current = tmp;
				if (current->type == D_QUOTE || current->type == S_QUOTE)
				{
					tmp = current->next;
					remove_token(tokens, current);
					current = tmp;
				}
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

void	remove_whitespace(t_token **tokens)
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

void	post_expander(void)
{
	remove_quotes(&g_minishell->tokens);
	// print_tokens(g_minishell->tokens);
}
