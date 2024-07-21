/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 14:38:55 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/21 22:45:00 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_special(char c)
{
	return (c == '\'' || c == '"');
}

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
		{
			return (1);
		}
		else
			return (0);
	}
	return (0);
}
