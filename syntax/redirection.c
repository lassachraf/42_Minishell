/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 18:38:34 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/08 19:05:05 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_right_valid1(t_type type)
{
	return (type == S_QUOTE || type == D_QUOTE || type == ASTERISK
		|| type == DOLLAR || type == WORD);
}

int	is_redirection(t_type type)
{
	return ((type >= LL_REDIR && type <= R_REDIR));
}

int	syntax_second_phase(t_token *token)
{
	if (is_redirection(token->type))
	{
		if (!token->next || token->next->type == END)
		{
			print_errors("syntax error near unexpected token `newline`");
			return (-1);
		}
		else if (token->type == L_REDIR && token->next 
			&& token->next->type == R_REDIR)
		{
			print_errors("syntax error near unexpected token `newline`");
			return (-1);
		}
		else if (!is_right_valid1(token->next->type))
		{
			ft_putstr_fd(RED "badashell1$ : ", 2);
			ft_putstr_fd("syntax error near unexpected token `", 2);
			ft_putstr_fd(token->next->value, 2);
			ft_putstr_fd("`\n" RESET, 2);
			return (-1);
		}
	}
	return (0);
}
