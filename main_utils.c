/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 02:03:45 by alassiqu          #+#    #+#             */
/*   Updated: 2024/08/14 10:32:15 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

char	*shlvl_hepler(char *shlvl)
{
	char	*new_shlvl;
	int		tmp;
	int		i;

	i = 0;
	tmp = ft_atoi(shlvl) + 1;
	while (shlvl[i] && ft_isdigit(shlvl[i]))
		i++;
	if (shlvl[i])
		new_shlvl = ft_strdup("1");
	else if (tmp < 0)
		new_shlvl = ft_strdup("0");
	else if (tmp >= 1000)
	{
		new_shlvl = ft_itoa(tmp);
		gc_add(g_minishell, new_shlvl);
		ft_putstr_fd(RED "badashell$ : warning : shell level (", 2);
		ft_putstr_fd(new_shlvl, 2);
		ft_putstr_fd(") too high, resetting to 1", 2);
		ft_putstr_fd("\n" RESET, 2);
		new_shlvl = ft_strdup("1");
	}
	else
		new_shlvl = ft_itoa(tmp);
	return (new_shlvl);
}

void	reset_fds(void)
{
	if (dup2(g_minishell->stdout, 1) == -1)
		perror("dup2 stdout");
	if (dup2(g_minishell->stdin, 0) == -1)
		perror("dup2 stdin");
}

void	clean_and_set(void)
{
	char	*exit_stat;

	gc_free_all(g_minishell);
	unlink_docs(g_minishell->docs);
	exit_stat = ft_itoa(g_minishell->exit_s);
	set_env_var(g_minishell->our_env, "?", exit_stat);
	free(exit_stat);
}

void	exit_and_set(void)
{
	char	*exit;

	exit = ft_itoa(g_minishell->exit_s);
	set_env_var(g_minishell->our_env, "?", exit);
	free(exit);
}

int	wait_last(void)
{
	int		fail;
	int		x;

	fail = -1;
	fail = waitpid(g_minishell->last_child, &x, 0);
	if (fail == -1)
		return (exit_and_set(), -1);
	if (x == 131)
	{
		g_minishell->exit_s = x;
		ft_putstr_fd("Quit (core dumped)\n", 2);
		return (exit_and_set(), -1);
	}
	if (WIFEXITED(x))
		g_minishell->exit_s = WEXITSTATUS(x);
	if (g_minishell->exit_s == 130 && x == 2)
		ft_putstr_fd("\n", 2);
	return (exit_and_set(), -1);
}
