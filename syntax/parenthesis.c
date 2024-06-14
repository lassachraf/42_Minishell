/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthesis.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 21:27:44 by alassiqu          #+#    #+#             */
/*   Updated: 2024/06/06 16:27:20 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	nb_quotes(void)
{
	t_token	*token;
	int		s_count;
	int		d_count;

	token = g_minishell->tokens;
	s_count = 0;
	d_count = 0;
	while (token)
	{
		if (token->type == S_QUOTE)
			s_count++;
		else if (token->type == D_QUOTE)
			d_count++;
		token = token->next;
	}
	if (s_count % 2 != 0)
		return (print_errors("unexpected EOF while looking for matching `\'`"),
			-1);
	if (d_count % 2 != 0)
		return (print_errors("unexpected EOF while looking for matching `\"`"),
			-1);
	return (0);
}

int	nb_paren(void)
{
	t_token	*token;
	int		l_count;
	int		r_count;

	token = g_minishell->tokens;
	l_count = 0;
	r_count = 0;
	while (token)
	{
		if (token->type == L_PAREN)
			l_count++;
		else if (token->type == R_PAREN)
			r_count++;
		token = token->next;
	}
	if (l_count > 0 && r_count == 0)
		return (print_errors("unexpected EOF while looking for matching `)'"),
			-1);
	else if (l_count > 0 && r_count > 0 && l_count != r_count)
		return (print_errors("syntax error near unexpected token `)'"),
			-1);
	return (0);
}

int	is_right_valid2(t_type type)
{
	return (type == S_QUOTE || type == D_QUOTE || type == ASTERISK
		|| type == DOLLAR || type == WORD || type == L_PAREN || type == R_PAREN
		|| (type >= LL_REDIR && type <= R_REDIR));
}

int	check_left_parenthesis(t_token *token)
{
	if (!(!token->prev || token->prev->type == L_PAREN
			|| is_pipe_or_and(token->prev->type)))
	{
		print_errors("syntax error near unexpected token `('");
		return (-1);
	}
	else if (!token->next || !is_right_valid2(token->next->type))
	{
		print_errors("syntax error near unexpected token `('");
		return (-1);
	}
	return (0);
}

int	syntax_third_phase(t_token *token)
{
	if (token->type == L_PAREN)
	{
		if (check_left_parenthesis(token) == -1)
			return (-1);
	}
	else if (token->type == R_PAREN)
	{
		if (check_right_parenthesis(token) == -1)
			return (-1);
	}
	return (0);
}
