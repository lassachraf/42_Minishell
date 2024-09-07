/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 17:48:42 by alassiqu          #+#    #+#             */
/*   Updated: 2024/09/06 18:36:16 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_errors(char *message)
{
	ft_putstr_fd(RED "badashell$ : ", 2);
	ft_putstr_fd(message, 2);
	ft_putstr_fd("\n" RESET, 2);
}

t_token	*skip_whitespace_token(t_token *token)
{
	if (token->type == WHITESPACE)
	{
		while (token && token->type == WHITESPACE)
			token = token->next;
	}
	return (token);
}

void	check_hd_expand(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type == LL_REDIR)
		{
			tokens = tokens->next;
			tokens = skip_whitespace_token(tokens);
			if (tokens->type == WORD)
			{
				if (tokens->quoted != 0)
					tokens->hd_expand = 0;
				else
					tokens->hd_expand = 1;
			}
			else
				tokens->hd_expand = 0;
		}
		else if (tokens->type == WORD && is_there_whitespaces(tokens->value))
			tokens->avoid = 1;
		tokens = tokens->next;
	}
}


// $PWD "$PWD"        >> 1 