/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 14:38:55 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/15 15:41:46 by baouragh         ###   ########.fr       */
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

void	skip_spaces(char **line)
{
	while (**line && ft_isspace(**line))
		(*line)++;
}

int	special_case(t_token *prev, t_token *curr, t_token *next)
{
	// printf("special >>>>>>>>>>>> curr : %d, next : %d\n", curr->type, next->type);
	if ((curr->type == S_QUOTE || curr->type == D_QUOTE)
		&& curr->type == next->type)
	{
		if (!prev || (prev && prev->type >= WORD && prev->type <= OR)) // pipe 
		{
			return (1);
		}
		else
			return (0);
	}
	return (0);
}
