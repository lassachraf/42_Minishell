/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthesis_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 19:40:46 by alassiqu          #+#    #+#             */
/*   Updated: 2024/06/10 14:48:41 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_left_valid3(t_type type)
{
	return ((type >= L_PAREN && type <= DOLLAR) || type == WORD);
}

int	is_right_valid3(t_type type)
{
	return (is_pipe_or_and(type) || type == R_PAREN || type == END);
}

int	check_right_parenthesis(t_token *token)
{
	if (!token->prev || !is_left_valid3(token->prev->type))
	{
		print_errors("syntax error near unexpected token `)'");
		return (-1);
	}
	else if (!token->next || !is_right_valid3(token->next->type))
	{
		if (!is_right_valid3(token->next->type))
		{
			ft_putstr_fd(RED "badashell$ : ", 2);
			ft_putstr_fd("syntax error near unexpected token `", 2);
			ft_putstr_fd(token->next->value, 2);
			ft_putstr_fd("`\n" RESET, 2);
			return (-1);
		}
	}
	return (0);
}
