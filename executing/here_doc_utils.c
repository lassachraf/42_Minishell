/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 09:08:11 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/27 15:32:24 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	do_here_doc(char *buf, char *limiter, int fd, int *pipe)
{
	char	*lines;
	int		old_count;

	old_count = g_minishell->lines;
	while (1)
	{
		signal(SIGINT, here_doc_sig);
		read_buf(&buf);
		if (!write_or_break(fd, limiter, buf, old_count))
			break ;
	}
	lines = ft_itoa(g_minishell->lines);
	printf("%s\n",lines);
	write(pipe[1], lines, ft_strlen(lines));
	free(lines);
	close(pipe[0]);
}
