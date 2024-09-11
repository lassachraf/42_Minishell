/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 15:33:43 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/09 16:46:10 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	execute_and_or(t_node *node)
{
	if (!node)
		return ;
	if (node->data.pair.type == OR)
	{
		executer(node->data.pair.left, NULL, NULL);
		wait_last();
		wait_all();
		if (g_minishell->exit_s && g_minishell->exit_s != 130)
			executer(node->data.pair.right, NULL, NULL);
	}
	else if (node->data.pair.type == AND)
	{
		executer(node->data.pair.left, NULL, NULL);
		wait_last();
		wait_all();
		if (!g_minishell->exit_s && g_minishell->exit_s != 130)
			executer(node->data.pair.right, NULL, NULL);
	}
}

void	pipe_left(t_node *node, int *pfd, int *fd_io)
{
	if (!node)
		return ;
	if (node->type != STRING_NODE)
	{
		if (node->type == PAIR_NODE)
		{
			g_minishell->last_child = fork();
			if (!g_minishell->last_child)
			{
				clean_fds(g_minishell->ast);
				close_fds(pfd, fd_io);
				execute_pair(node, pfd, fd_io);
				wait_last();
				while (waitpid(-1, NULL, 0) != -1)
					;
				save_status_clean();
			}
		}
		else
			execute_redires(node->data.redir, NULL, NULL);
	}
	else
		do_pipe(node, pfd, fd_io);
}

void	pipe_right(t_node *node, int *pfd, int *fd_io)
{
	if (!node)
		return ;
	if (node->type != STRING_NODE)
	{
		if (node->data.pair.type == AND)
			executer(node, pfd, fd_io);
		else if (node->data.pair.type == OR)
			executer(node, pfd, fd_io);
		else if (node->type == REDIR_NODE)
			execute_redires(node->data.redir, NULL, NULL);
		else
			executer(node, pfd, fd_io);
	}
	else
		do_pipe(node, pfd, fd_io);
}

void	execute_pair(t_node *node, int *pfd_2, int *fd_io_2)
{
	if (pfd_2 || fd_io_2)
		exe_old_pfd(node, pfd_2, fd_io_2);
	else
		exe_non_opfd(node);
}

void	executer(t_node *node, int *pfd, int *fd_io)
{
	if (!node)
		return ;
	if (node->type == STRING_NODE)
	{
		if (node->data.cmd && node->data.cmd->is_block
			&& ft_is_builtin((char *)node->data.cmd->content))
			select_and_execute(node, STRING_NODE, pfd, fd_io);
		else
			execute_cmd(node, pfd, fd_io);
	}
	else if (node->type == PAIR_NODE)
	{
		if (node->data.pair.is_block)
			select_and_execute(node, PAIR_NODE, pfd, fd_io);
		else
			execute_pair(node, pfd, fd_io);
	}
	else if (node->type == REDIR_NODE)
		execute_redires(node->data.redir, pfd, fd_io);
}
