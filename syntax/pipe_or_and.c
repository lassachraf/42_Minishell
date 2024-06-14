/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_or_and.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 20:12:50 by alassiqu          #+#    #+#             */
/*   Updated: 2024/06/13 12:28:13 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_left_valid(t_type type)
{
	return (type == S_QUOTE || type == D_QUOTE || type == ASTERISK
		|| type == DOLLAR || type == WORD || type == R_PAREN);
}

int	is_right_valid(t_type type)
{
	return (type == S_QUOTE || type == D_QUOTE || type == ASTERISK
		|| type == DOLLAR || type == WORD || type == L_PAREN
		|| type == LL_REDIR || type == RR_REDIR || type == L_REDIR
		|| type == R_REDIR);
}

int	is_pipe_or_and(t_type type)
{
	return (type == PIPE || type == OR || type == AND);
}

int	first_checker_left(t_token *token)
{
	if (is_pipe_or_and(token->type) && (!token->prev
			|| !is_left_valid(token->prev->type)))
	{
		ft_putstr_fd(RED "badashell$ : syntax error near unexpected token `",
			2);
		ft_putstr_fd(token->value, 2);
		ft_putstr_fd("`\n" RESET, 2);
		return (-1);
	}
	return (0);
}

int	first_checker_right(t_token *token)
{
	if (is_pipe_or_and(token->type) && (!token->next
			|| !is_right_valid(token->next->type)))
	{
		ft_putstr_fd(RED "badashell$ : syntax error near unexpected token `",
			2);
		ft_putstr_fd(token->next->value, 2);
		ft_putstr_fd("`\n" RESET, 2);
		return (-1);
	}
	return (0);
}
