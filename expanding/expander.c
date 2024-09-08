/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 11:11:46 by alassiqu          #+#    #+#             */
/*   Updated: 2024/09/08 18:37:35 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	add_token_middle(t_token **tokens, t_token *new_token,
		t_token *prev_token)
{
	if (!tokens || !new_token)
	{
		return ;
	}
	if (prev_token == NULL)
	{
		new_token->next = *tokens;
		if (*tokens)
			(*tokens)->prev = new_token;
		*tokens = new_token;
		return ;
	}
	new_token->prev = prev_token;
	new_token->next = prev_token->next;
	if (prev_token->next)
		prev_token->next->prev = new_token;
	prev_token->next = new_token;
}

void	check_for_value(t_token **tokens, char *new, int flag)
{
	t_token	*tmp;

	tmp = NULL;
	if (!new)
	{
		tmp = (*tokens)->next;
		remove_token(&g_minishell->tokens, (*tokens));
		(*tokens) = tmp;
	}
	else if (contains_space(new))
	{
		printf("allo\n");
		handle_space(*tokens, new, flag);
	}
	else
		(*tokens)->value = new;
}

t_token	*word_helper(t_token *tokens)
{
	char	*new_value;

	new_value = NULL;
	if (tokens->prev && tokens->prev->type == LL_REDIR)
		return (tokens->next);
	if (tokens->wd_expand)
	{
		new_value = helper_expander(tokens->value);
		check_for_value(&tokens, new_value, tokens->quoted);
	}
	return (tokens->next);
}

void	expand_dollar(void)
{
	t_token	*tokens;

	tokens = g_minishell->tokens;
	while (tokens)
	{
		if (tokens->value && is_separator_2(tokens->value))
			break ;
		if (tokens->type == WORD && tokens->value
			&& !ft_strcmp(tokens->value, "export"))
		{
			if (export_help(&tokens, tokens->next->next_space))
				break ;
		}
		else if (tokens->type == WORD && tokens->value
			&& ft_strchr(tokens->value, '$') && tokens->quoted == 1)
		{
			tokens->value = helper_expander(tokens->value);
			tokens = tokens->next;
		}
		else if (tokens->type == WORD && tokens->value
			&& ft_strchr(tokens->value, '$') && tokens->quoted == 0)
			tokens = word_helper(tokens);
		else
			tokens = tokens->next;
	}
}

void	expanding(void)
{
	t_token	*tokens;

	tokens = g_minishell->tokens;
	while (tokens)
	{
		if (tokens->type == S_QUOTE)
		{
			tokens = tokens->next;
			while (tokens && tokens->type != S_QUOTE)
				tokens = tokens->next;
			tokens = tokens->next;
		}
		else if (tokens->type == WORD && ft_strchr(tokens->value, '~'))
		{
			tokens->value = custome_path(tokens->value);
			tokens = tokens->next;
		}
		else
			tokens = tokens->next;
	}
}
