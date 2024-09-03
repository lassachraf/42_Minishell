/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 09:08:11 by alassiqu          #+#    #+#             */
/*   Updated: 2024/09/03 09:58:09 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	do_here_doc(char *limiter, int fd, int *pipe, int expand_flag)
{
	char	*lines;
	int		old_count;
	char	*buf;

	buf = NULL;
	old_count = g_minishell->lines;
	while (1)
	{
		signal(SIGINT, here_doc_sig);
		read_buf(&buf, limiter, expand_flag);
		if (!write_or_break(fd, limiter, buf, old_count))
			break ;
	}
	lines = ft_itoa(g_minishell->lines);
	write(pipe[1], lines, ft_strlen(lines));
	free(lines);
	close(pipe[0]);
	g_minishell->exit_s = 0;
}

bool	output_redirs(t_redir *new)
{
	if (new->file[0] == '\0')
		return (print_err("No such file or directory", new->file), 1);
	if (!access(new->file, F_OK))
	{
		if (!access(new->file, W_OK))
			return (0);
		return (print_err("Permission denied", new->file), 1);
	}
	return (0);
}

bool	check_name(t_redir *new)
{
	struct stat	statbuf;

	statbuf.st_mode = 0;
	stat(new->file, &statbuf);
	if (S_ISDIR(statbuf.st_mode) == true)
		return (print_err("Is a directory", new->file), 1);
	if (new->fd == -1)
		return (print_err("ambiguous redirect", new->file), 1);
	if (new->type != L_REDIR)
		return (output_redirs(new));
	else if (!access(new->file, F_OK))
	{
		if (!access(new->file, R_OK))
			return (0);
		return (print_err("Permission denied", new->file), 1);
	}
	else
		return (print_err("No such file or directory", new->file), 1);
}

char	*build_file_name(char *join)
{
	char	**split;
	char	*name;
	char	*tmp;
	int		i;

	split = ft_split(ttyname(0), '/');
	name = NULL;
	i = 0;
	while (split[i])
	{
		tmp = name;
		name = ft_strjoin(name, split[i++]);
		free(tmp);
	}
	tmp = name;
	name = ft_strjoin(name, join);
	free(tmp);
	if (access(name, F_OK) != 0)
		return (free_double(split), name);
	else
	{
		tmp = name;
		return (free_double(split), free(tmp), ft_strjoin(name, join));
	}
	return (NULL);
}

int	open_hidden_file(int doc_num)
{
	char	*join;
	char	*path;
	char	*name;
	int		fd;

	join = ft_itoa(doc_num);
	path = build_file_name(join);
	name = ft_strjoin(PATH, path);
	fd = open(name, O_CREAT | O_RDWR, 0777);
	free(name);
	free(path);
	free(join);
	if (fd < 0)
	{
		perror("here_doc failed to get input");
		return (-1);
	}
	return (fd);
}
