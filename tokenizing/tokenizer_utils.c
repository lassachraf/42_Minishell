/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 14:38:55 by alassiqu          #+#    #+#             */
/*   Updated: 2024/08/10 18:44:37 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_separator(char *s)
{
	return (!ft_strncmp(s, "&&", 2) || *s == ' ' || *s == '\t' || *s == '<'
		|| *s == '>' || *s == '|' || *s == '(' || *s == ')');
}

int	special_case(t_token *prev, t_token *curr, t_token *next)
{
	if ((curr->type == S_QUOTE || curr->type == D_QUOTE)
		&& curr->type == next->type)
	{
		if (!prev || (prev && prev->type >= WORD && prev->type <= OR))
			return (1);
		else
			return (0);
	}
	return (0);
}

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

int	check_for_quote_close(char **line, int c)
{
	int	i;

	i = 0;
	while ((*line)[i])
	{
		if ((*line)[i] == c)
			return (1);
		i++;
	}
	return (0);
}
