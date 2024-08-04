/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 17:48:42 by alassiqu          #+#    #+#             */
/*   Updated: 2024/08/03 21:50:00 by alassiqu         ###   ########.fr       */
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
				tokens = tokens->next;
				if (tokens->type == S_QUOTE || tokens->type == D_QUOTE)
					tokens->prev->hd_expand = 0;
				else
					tokens->prev->hd_expand = 1;
			}
			else
				tokens->hd_expand = 0;
		}
		tokens = tokens->next;
	}
}
