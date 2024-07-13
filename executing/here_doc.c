/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 16:15:09 by baouragh          #+#    #+#             */
/*   Updated: 2024/07/13 15:01:58 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	open_hidden_file(int doc_num)
{
	int	fd;
	char *join;
	char *name;

	join  = ft_itoa(doc_num);
	name = ft_strjoin(PATH, join);
	fd = open(name, O_CREAT | O_RDWR | O_APPEND, 0777);
	free(name);
	free(join);
	if (fd < 0)
	{
		perror("1 --> here_doc failed to get input");
		exit(EXIT_FAILURE);
	}
	return(fd);
}

static int	re_open_hidden_file(int doc_num)
{
	int	fd;
	char *join;
	char *name;

	join  = ft_itoa(doc_num);
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

static int	write_or_break(int fd, char *limiter, char *buf)
{
	int	doc_len;
	int	buf_len;

	if(!buf)
		return(0);
	doc_len = ft_strlen(limiter);
	buf_len = ft_strlen(buf);
	if (!ft_strncmp (limiter, buf, buf_len + doc_len))
		return (0);
	write(fd, buf, buf_len);
	write(fd,"\n",1);
	return (1);
}

static void ft_sig_handler(int sig)
{
	if (sig == SIGINT)
	{
		// sig = 0;
		printf("DONE\n");		
		signal(SIGINT, SIG_IGN);
		g_minishell->exit_s = 130;
	}
}

void	h_signals(void)
{
	// rl_catch_signals = 0;
	signal(SIGINT, ft_sig_handler);
	signal(SIGQUIT, SIG_IGN);
}
static void	read_buf(char **buf)
{
	*buf = readline(">");
	if(*buf)
		gc_add(g_minishell, *buf);
}

void handler(int sig)
{
	if (sig != SIGINT)
		return ;
	printf("\n");
	g_minishell->exit_s = 130;
	exit(130);
}

int	here_doc(char *limiter ,int doc_num)
{
	char	*buf;
	int		fd;
	int		fd_hidden;
	int 	id;

	fd_hidden = -1;
	fd = open_hidden_file(doc_num);
	id = fork();
	if (!id)
	{
	while (1)
	{
		signal(SIGINT, handler);
		read_buf(&buf);
		if (!write_or_break(fd, limiter, buf)) // ctrl + D --> EOF , limiter
			break;
	}
		exit(0);
	}
	else
	{
		wait_and_get();
		if(!g_minishell->exit_s)
		{
			fd_hidden = re_open_hidden_file(doc_num);
			return(fd_hidden);
		}
	}
	return(-1);
}
