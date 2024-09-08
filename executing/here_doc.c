/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 16:15:09 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/07 18:12:10 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	re_open_hidden_file(int doc_num)
{
	char	*path;
	char	*name;
	int		fd;

	path = get_filename(doc_num);
	if (!path)
		return (-1);
	name = ft_strjoin(PATH, path);
	fd = open(name, O_RDONLY);
	free(name);
	if (fd < 0)
	{
		perror("here_doc failed to get input");
		return (-1);
	}
	return (fd);
}

int	write_or_break(int fd, char *limiter, char *buf, int count)
{
	int		doc_len;
	int		buf_len;
	char	*count_s;

	if (!buf)
	{
		ft_putstr_fd("warning: here-document at line ", 2);
		count_s = ft_itoa(count);
		ft_putstr_fd(count_s, 2);
		free(count_s);
		ft_putstr_fd(" delimited by end-of-file (wanted `", 2);
		ft_putstr_fd(limiter, 2);
		ft_putstr_fd("')", 2);
		ft_putstr_fd("\n", 2);
		return (0);
	}
	doc_len = ft_strlen(limiter);
	buf_len = ft_strlen(buf);
	if (!ft_strncmp(limiter, buf, buf_len + doc_len))
		return (0);
	write(fd, buf, buf_len);
	write(fd, "\n", 1);
	return (1);
}

void	read_buf(char **buf, char *limiter, int expand_flag)
{
	g_minishell->lines++;
	*buf = readline("> ");
	if (*buf)
	{
		if (ft_strchr(*buf, '$') && expand_flag && ft_strcmp(*buf, limiter)
			&& *limiter)
		{
			*buf = helper_expander(*buf);
			if (!*buf)
			{
				*buf = ft_strdup("");
				gc_add(g_minishell, *buf);
			}
		}
		else
			gc_add(g_minishell, *buf);
	}
}

void	get_lines_count(int *pipe)
{
	char	buf[2048];
	int		br;

	close(pipe[1]);
	br = read(pipe[0], &buf, 2048);
	buf[br] = '\0';
	g_minishell->lines = ft_atoi(buf);
	close(pipe[0]);
}

int	here_doc(char *limiter, int doc_num, int expand_flag)
{
	int	fd;

	g_minishell->her_pfd = malloc(sizeof (int) * 2);
	gc_add(g_minishell, g_minishell->her_pfd);
	open_pipe(g_minishell->her_pfd);
	fd = open_hidden_file(doc_num);
	if (fd < 0)
		return (-1);
	g_minishell->last_child = fork();
	if (!g_minishell->last_child)
	{
		clean_fds(g_minishell->ast);
		do_here_doc(limiter, fd, g_minishell->her_pfd, expand_flag);
		close(fd);
		save_status_clean();
	}
	else
	{
		wait_and_get();
		if (g_minishell->exit_s != 130)
			get_lines_count(g_minishell->her_pfd);
		if (!g_minishell->exit_s)
			return (close(fd), re_open_hidden_file(doc_num));
	}
	return (close(fd), -1);
}
