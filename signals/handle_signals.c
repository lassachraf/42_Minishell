/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 21:05:29 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/13 14:52:24 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_sigint_handler(int sig)
{
	if (sig == SIGINT)
	{
		ft_putstr_fd("^C\n", 1);
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

// void	ft_sigquit_handler(int sig)
// {
// 	(void)sig;
// 	ft_putstr_fd("Quit (core dumped)\n", 1);
// 	// rl_on_new_line();
// 	rl_replace_line("", 0);
// 	rl_redisplay();
// 	set_env_var(g_minishell->our_env, "?", "130");
// }

void	signals(void)
{
	signal(SIGINT, ft_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void hand(int sig)
{
	if(sig != SIGQUIT)
		return;
	printf("Quit (core dumped)\n");
}

void hand2(int sig)
{
	if(sig != SIGINT)
		return;
	printf("\n");
}
