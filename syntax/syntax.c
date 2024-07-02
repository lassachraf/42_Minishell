/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:13:52 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/02 17:20:59 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	syntax_first_phase(t_token *token)
{
	if (first_checker_left(token) == -1)
		return (-1);
	if (first_checker_right(token) == -1)
		return (-1);
	return (0);
}

int	count_nb_here_doc(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens)
	{
		if (tokens->type == LL_REDIR)
			count++;
		tokens = tokens->next;
	}
	if (count > 16)
	{
		print_errors("maximum here-document count exceeded");
		return (-1);
	}
	else
		return (0);
}

int	general_check(void)
{
	if (nb_paren())
		return (-1);
	if (nb_quotes())
		return (-1);
	if (count_nb_here_doc(g_minishell->tokens))
		return (-1);
	return (0);
}

int	syntax(void)
{
	t_token	*token;

	remove_whitespace(&g_minishell->tokens);
	token = g_minishell->tokens;
	while (token)
	{
		if (syntax_first_phase(token) || syntax_second_phase(token)
			|| syntax_third_phase(token))
		{
			set_env_var(g_minishell->our_env, "?", "2");
			gc_free_all(g_minishell);
			return (-1);
		}
		token = token->next;
	}
	if (general_check() == -1)
	{
		set_env_var(g_minishell->our_env, "?", "2");
		gc_free_all(g_minishell);
		return (-1);
	}
	return (0);
}
