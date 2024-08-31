/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 20:58:27 by alassiqu          #+#    #+#             */
/*   Updated: 2024/08/31 21:10:40 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

t_minishell	*g_minishell;

void	wait_all(void)
{
	while (waitpid(-1, NULL, 0) != -1)
	{
		if (g_minishell->exit_s == 130)
		{
			ft_putstr_fd("\n", 2);
			g_minishell->exit_s = 0;
		}
	}
}

void	increment_shlvl(void)
{
	char	*shlvl;
	char	*new_shlvl;

	shlvl = get_env_var(g_minishell->our_env, "SHLVL");
	if (!shlvl)
		return ;
	new_shlvl = shlvl_hepler(shlvl);
	gc_add(g_minishell, new_shlvl);
	set_env_var(g_minishell->our_env, "SHLVL", new_shlvl);
}

int	init_minishell(char **env)
{
	g_minishell = malloc(sizeof(t_minishell));
	if (!g_minishell)
		return (0);
	ft_bzero(g_minishell, sizeof(t_minishell));
	g_minishell->stdin = dup(0);
	g_minishell->stdout = dup(1);
	if (g_minishell->stdin == -1 || g_minishell->stdout == -1)
		return (perror("dup failed in init"), 0);
	if (env && *env)
	{
		g_minishell->our_env = dup_env(env);
		increment_shlvl();
	}
	else
		g_minishell->our_env = special_dup_env();
	add_env_var(g_minishell->our_env, "?", "0");
	set_as_invisible(g_minishell->our_env, "?");
	set_as_unexported(g_minishell->our_env, "?");
	set_env_var(g_minishell->our_env, "_", "]");
	set_as_unexported(g_minishell->our_env, "_");
	signals();
	return (1);
}

void	ft_readline(void)
{
	int	exit_status;

	exit_status = 0;
	g_minishell->lines++;
	g_minishell->docs = 0;
	g_minishell->line = readline(PROMPT);
	gc_add(g_minishell, g_minishell->line);
	if (!g_minishell->line)
	{
		exit_status = g_minishell->exit_s;
		ft_putstr_fd("exit\n", 1);
		cleanup_minishell();
		exit(exit_status);
	}
	if (g_minishell->line[0])
		add_history(g_minishell->line);
}

int	main(int argc, char **argv, char **env)
{
	(void)argc, (void)argv;
	if (!init_minishell(env))
		return (1);
	while (1)
	{
		signals();
		ft_readline();
		signal(SIGINT, ft_sigint);
		g_minishell->tokens = tokenizer();
		if (!g_minishell->tokens || syntax() == -1)
			continue ;
		g_minishell->ast = parsing();
		if (!g_minishell->ast)
			continue ;
		if (scan_and_set(g_minishell->ast))
			executer(g_minishell->ast, NULL);
		reset_fds();
		wait_last();
		wait_all();
		clean_and_set();
	}
	return (cleanup_minishell(), 0);
}
