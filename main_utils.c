/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 02:03:45 by alassiqu          #+#    #+#             */
/*   Updated: 2024/08/06 18:50:12 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	clean_fds(t_node *ast)
{
	t_list	*red_lst;
	t_redir	*red;

	if (ast->type == PAIR_NODE)
	{
		clean_fds(ast->data.pair.left);
		clean_fds(ast->data.pair.right);
	}
	else if (ast->type == REDIR_NODE)
	{
		red_lst = ast->data.redir;
		while (red_lst)
		{
			red = red_lst->content;
			close(red->fd);
			red_lst = red_lst->next;
		}
	}
}

void	clean_and_set(void)
{
	char	*exit_stat;

	dup2(g_minishell->stdout, 1);
	dup2(g_minishell->stdin, 0);
	gc_free_all(g_minishell);
	unlink_docs(g_minishell->docs);
	exit_stat = ft_itoa(g_minishell->exit_s);
	set_env_var(g_minishell->our_env, "?", exit_stat);
	free(exit_stat);
}

int	wait_last(void)
{
	int		fail;
	int		x;
	char	*exit;

	fail = -1;
	exit = NULL;
	fail = waitpid(g_minishell->last_child, &x, 0);
	if (x == 131)
	{
		g_minishell->exit_s = x;
		ft_putstr_fd("Quit (core dumped)\n", 2);
		return (1);
	}
	if (WIFEXITED(x))
		g_minishell->exit_s = WEXITSTATUS(x);
	if (g_minishell->exit_s == 130 && x == 2)
		ft_putstr_fd("\n", 2);
	exit = ft_itoa(g_minishell->exit_s);
	set_env_var(g_minishell->our_env, "?", exit);
	return (free(exit), fail);
}
