/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:26:22 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/09 14:51:44 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	open_hidden_file(void)
{
	int fd;
	fd = open("/var/tmp/tmp.txt", O_CREAT | O_RDWR | O_APPEND, 0777);
	if (fd < 0)
	{
		perror("1 --> here_doc failed to get input");
		exit(EXIT_FAILURE);
	}
	return(fd);
}

static int	re_open_hidden_file(char *name)
{
	int	fd;

	fd = open(name, O_RDONLY);
	if (fd < 0)
	{
		perror("2 --> here_doc failed to get input");
		exit(EXIT_FAILURE);
	}
	return (fd);
}

static int	write_or_break(int fd, char *limiter, char *buf)
{
	int	buf_len;

	if(!buf)
		return(0);
	buf_len = ft_strlen(buf);
	if (buf[0] == '\0' || !ft_strncmp (limiter, buf, buf_len))
		return (0);
	write(fd, buf, buf_len);
	write(fd,"\n",1);
	return (1);
}

static void	read_buf(char **buf)
{
	*buf = readline(">");
	if(*buf)
		gc_add(g_minishell, *buf);
}

static void wait_and_get(void)
{
	char *exit;

	wait(&g_minishell->exit_s);
	// printf("befor '%d'\n",g_minishell->exit_s);
	if (WIFEXITED(g_minishell->exit_s))
		g_minishell->exit_s = WEXITSTATUS(g_minishell->exit_s);
	exit = ft_itoa(g_minishell->exit_s);
	// printf("after '%s'\n",exit);
	if(!exit)
		return(print_errors("ERROR WITH FT_ITOA\n"));
	set_env_var(g_minishell->our_env, "?", exit);
	free(exit);
}
static void	ft_sigint_handler(int sig)
{
	if (sig == SIGINT)
	{
		sig = 0;
		exit(130);
	}
}

void	h_signals(void)
{
	rl_catch_signals = 0;
	signal(SIGINT, ft_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

int	here_doc(char *limiter)
{
	char	*buf;
	int		fd;
	int		fd_hidden;
	int 	id;
	
	id = fork();
	if (!id)
	{
		h_signals();
		fd = open_hidden_file();
		while (1)
		{
			read_buf(&buf);
			if (!write_or_break(fd, limiter, buf))
				break;
		}
		exit(0);
	}
	else
	{
		// after_signals();
		wait_and_get();
		if(!g_minishell->exit_s)
		{
			// printf("re open !!\n");
			fd_hidden = re_open_hidden_file("/var/tmp/tmp.txt");
			return(fd_hidden);
			// dup_2(fd_hidden, 0);
		}
		else if(g_minishell->exit_s == 130)
		{
			printf("CTRL + C , remove tmp file !!\n");
			unlink("/var/tmp/tmp.txt");
		}
	}
	return(-1);
}
