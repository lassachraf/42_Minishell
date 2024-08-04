/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 02:03:45 by alassiqu          #+#    #+#             */
/*   Updated: 2024/08/04 02:13:25 by alassiqu         ###   ########.fr       */
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

	clean_fds(g_minishell->ast);
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
	fail = waitpid(g_minishell->last_child, &x, 0);
	if (x == 131)
		ft_putstr_fd("Quit (core dumped)\n", 2);
	if (x == 130)
		ft_putstr_fd("\n", 2);
	if (WIFEXITED(x))
		g_minishell->exit_s = WEXITSTATUS(x);
	exit = ft_itoa(g_minishell->exit_s);
	set_env_var(g_minishell->our_env, "?", exit);
	return (free(exit), fail);
}
