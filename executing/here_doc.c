/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 16:15:09 by baouragh          #+#    #+#             */
/*   Updated: 2024/07/17 22:45:54 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	open_hidden_file(int doc_num)
{
	char	*join;
	char	*name;
	int		fd;

	join = ft_itoa(doc_num);
	name = ft_strjoin(PATH, join);
	fd = open(name, O_CREAT | O_RDWR | O_APPEND, 0777);
	free(name);
	free(join);
	if (fd < 0)
	{
		perror("1 --> here_doc failed to get input");
		exit(EXIT_FAILURE);
	}
	return (fd);
}

int	re_open_hidden_file(int doc_num)
{
	char	*join;
	char	*name;
	int		fd;

	join = ft_itoa(doc_num);
	name = ft_strjoin(PATH, join);
	fd = open(name, O_RDONLY);
	free(name);
	free(join);
	if (fd < 0)
	{
		perror("2 --> here_doc failed to get input");
		exit(EXIT_FAILURE);
	}
	return (fd);
}

int	write_or_break(int fd, char *limiter, char *buf)
{
	int	doc_len;
	int	buf_len;

	if (!buf)
		return (0);
	doc_len = ft_strlen(limiter);
	buf_len = ft_strlen(buf);
	if (!ft_strncmp(limiter, buf, buf_len + doc_len))
		return (0);
	write(fd, buf, buf_len);
	write(fd, "\n", 1);
	return (1);
}

void	read_buf(char **buf, int expand)
{
	*buf = readline("> ");
	if (*buf)
	{
		gc_add(g_minishell, *buf);
		if (ft_strchr(*buf, '$') && expand)
			here_doc_expanding(buf);
	}
}

int	here_doc(char *limiter, int doc_num, int expand)
{
	char	*buf;
	int		id;
	int		fd;
	int		fd_hidden;

	buf = NULL;
	fd_hidden = -1;
	fd = open_hidden_file(doc_num);
	id = fork();
	if (!id)
	{
		do_here_doc(buf, limiter, fd, expand);
		exit(0);
	}
	else
	{
		wait_and_get();
		if (!g_minishell->exit_s)
		{
			fd_hidden = re_open_hidden_file(doc_num);
			return (fd_hidden);
		}
	}
	return (-1);
}
