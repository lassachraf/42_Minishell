/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 21:05:29 by alassiqu          #+#    #+#             */
/*   Updated: 2024/08/08 00:43:17 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_sigint_handler(int sig)
{
	if (sig == SIGINT)
	{
		ft_putstr_fd("\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		set_env_var(g_minishell->our_env, "?", "130");
		g_minishell->exit_s = 130;
	}
	else if (sig == SIGQUIT)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		set_env_var(g_minishell->our_env, "?", "131");
		g_minishell->exit_s = 131;
	}
}

void	ft_sigint(int sig)
{
	if (sig != SIGINT)
		return ;
	g_minishell->exit_s = 130;
}

void	here_doc_sig(int sig)
{
	if (sig != SIGINT)
		return ;
	ft_putstr_fd("\n", 2);
	g_minishell->exit_s = 130;
	exit(130);
}

void	signals(void)
{
	signal(SIGINT, ft_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}
