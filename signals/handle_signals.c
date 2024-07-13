/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 21:05:29 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/13 09:51:13 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_sigint_handler(int sig)
{
	(void)sig;
	ft_putstr_fd("^C\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	set_env_var(g_minishell->our_env, "?", "130");
}

void	ft_sigquit_handler(int sig)
{
	(void)sig;
	ft_putstr_fd("Quit (core dumped)\n", 1);
	// rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	set_env_var(g_minishell->our_env, "?", "130");
}

void	signals(void)
{
	// rl_catch_signals = 0;
	signal(SIGINT, ft_sigint_handler);
	signal(SIGQUIT, ft_sigquit_handler);
}

// void	signal_init(void)
// {
// 	signal(SIGINT, ft_sigint_handler);
// 	signal(SIGQUIT, ft_sigquit_handler);
// }