/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 08:08:03 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/08 16:37:20 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	init_pipes(int *pfd, int *fd_io)
{
	if (open_pipe(pfd) == -1)
		return (-1);
	fd_io[0] = dup(0);
	if (fd_io[0] < 0)
	{
		fd_closer(pfd, 2);
		return (-1);
	}
	fd_io[1] = dup(1);
	if (fd_io[1] < 0)
	{
		fd_closer(pfd, 2);
		close(fd_io[0]);
		return (-1);
	}
	return (0);
}

void	left_pipe(t_node *node, int *pfd, int *fd_io)
{
	dup_2(pfd[1], 1);
	pipe_left(node->data.pair.left, pfd, fd_io);
	if (dup2(fd_io[1], 1) == -1)
	{
		perror("dup2 fd_io[1]");
		fd_closer(pfd, 2);
		dup_2(fd_io[0], 0);
		dup_2(fd_io[1], 1);
		return ;
	}
	dup_2(pfd[0], 0);
}

void	exe_non_opfd(t_node *node)
{
	int	pfd[2];
	int	fd_io[2];

	if (node->data.pair.type == PIPE)
	{
		if (init_pipes(pfd, fd_io))
			return ;
		left_pipe(node, pfd, fd_io);
		pipe_right(node->data.pair.right, pfd, fd_io);
		dup_2(fd_io[0], 0);
		dup_2(fd_io[1], 1);
	}
	else
		execute_and_or(node);
}

void	exe_old_pfd(t_node *node, int *pfd_2, int *fd_io_2)
{
	int	pfd[2];
	int	fd_io[2];

	clean_fds(g_minishell->ast);
	close_fds(pfd_2, fd_io_2);
	g_minishell->last_child = fork();
	if (!g_minishell->last_child)
	{
		if (node->data.pair.type == PIPE)
		{
			if (init_pipes(pfd, fd_io))
				return ;
			left_pipe(node, pfd, fd_io);
			pipe_right(node->data.pair.right, pfd, fd_io);
			dup_2(fd_io[0], 0);
			dup_2(fd_io[1], 1);
			wait_last();
		}
		else
			execute_and_or(node);
		save_status_clean();
	}
}
