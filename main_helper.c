/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 18:10:17 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/09 09:55:57 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	clean_and_set(void)
{
	char	*exit_stat;

	if (g_minishell->her_pfd)
	{
		close(g_minishell->her_pfd[0]);
		close(g_minishell->her_pfd[1]);
	}
	g_minishell->size = 2;
	clean_fds(g_minishell->ast);
	unlink_docs(g_minishell->docs);
	exit_stat = ft_itoa(g_minishell->exit_s);
	free(exit_stat);
}

void	exit_and_set(void)
{
	char	*exit;

	exit = ft_itoa(g_minishell->exit_s);
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

char	*get_filename(int id)
{
	t_list	*list;

	list = g_minishell->files;
	while (list)
	{
		if (list->id == id)
			return ((char *)list->content);
		list = list->next;
	}
	return (NULL);
}
