/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 09:08:11 by alassiqu          #+#    #+#             */
/*   Updated: 2024/09/07 16:42:28 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	do_here_doc(char *limiter, int fd, int *pipe, int expand_flag)
{
	char	*lines;
	int		old_count;
	char	*buf;

	clean_fds(g_minishell->ast);
	close(g_minishell->stdin);
	g_minishell->stdin = fd;
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
	close(pipe[1]);
	g_minishell->exit_s = 0;
}

bool	output_redirs(t_redir *new)
{
	struct stat	statbuf;

	statbuf.st_mode = 0;
	stat(new->file, &statbuf);
	if (S_ISDIR(statbuf.st_mode) == true)
		return (print_err("Is a directory", new->file), 1);
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

void	set_filename(char *filename, unsigned char buff[], int length)
{
	int	val;
	int	i;

	i = -1;
	val = 0;
	while (++i < length)
	{
		val = buff[i] % 62;
		if (val < 26)
			filename[i] = 'a' + val;
		else if (val < 52)
			filename[i] = 'A' + (val - 26);
		else
			filename[i] = '0' + (val - 52);
	}
	filename[length] = '\0';
}

void	generate_random_filename(char *filename, int length)
{
	unsigned char	buffer[length];
	int				fd;

	fd = open("/dev/random", O_RDONLY);
	if (fd < 0)
	{
		perror("Failed to open /dev/random");
		exit(EXIT_FAILURE);
	}
	if (read(fd, buffer, length) != length)
	{
		perror("Failed to read from /dev/random");
		close(fd);
		exit(EXIT_FAILURE);
	}
	close(fd);
	set_filename(filename, buffer, length - 1);
}

char	*build_file_name(int id)
{
	t_list		*file;
	char		*name;
	char		nb[20];

	generate_random_filename(nb, 20);
	name = ft_strjoin("badashell_file_", nb);
	gc_add(g_minishell, name);
	file = ft_lstnew(name);
	file->id = id;
	gc_add(g_minishell, file);
	ft_lstadd_back(&g_minishell->files, file);
	return (name);
}

int	open_hidden_file(int doc_num)
{
	char	*path;
	char	*name;
	int		fd;

	path = build_file_name(doc_num);
	name = ft_strjoin(PATH, path);
	fd = open(name, O_CREAT | O_RDWR, 0777);
	free(name);
	if (fd < 0)
	{
		perror("here_doc failed to get input");
		return (-1);
	}
	return (fd);
}
