/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_tools.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:09:20 by baouragh          #+#    #+#             */
/*   Updated: 2024/08/10 19:38:23 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	open_pipe(int *pfd)
{
	if (pipe(pfd))
	{
		perror("pipe:");
		return (-1);
	}
	return (0);
}

int	dup_2(int old_fd, int new_fd)
{
	if (dup2(old_fd, new_fd) < 0)
	{
		perror("dup2: ");
		return (-1);
	}
	close(old_fd);
	return (0);
}

void	fd_duper(int *pfd, int mode)
{
	if (mode == 1)
	{
		close(pfd[1]);
		close(pfd[0]);
	}
	else
	{
		close(pfd[0]);
		if (dup2(pfd[1], 1) == -1)
			perror("dup2 fd_duper");
	}
}

char	*get_command(char *argv)
{
	char	*cmd;
	int		i;

	i = 0;
	while (!ft_isspace(argv[i]) && argv[i] != '\0')
		i++;
	cmd = malloc(i + 1);
	gc_add(g_minishell, cmd);
	if (!cmd)
	{
		perror("malloc");
		return (NULL);
	}
	ft_memmove(cmd, argv, i);
	cmd[i] = '\0';
	return (cmd);
}

char	*add_slash_cmd(char *path, char *cmd)
{
	char	*a_path;
	char	*fullpath;

	a_path = (ft_strjoin(path, "/"));
	fullpath = ft_strjoin(a_path, cmd);
	if (!access(fullpath, X_OK))
		return (free(a_path), fullpath);
	else
		return (free(fullpath), free(a_path), NULL);
}
