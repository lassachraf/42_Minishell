/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 09:08:11 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/17 22:45:40 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	do_here_doc(char *buf, char *limiter, int fd , int expand)
{
	while (1)
	{
		signal(SIGINT, here_doc_sig);
		read_buf(&buf, expand);
		if (!write_or_break(fd, limiter, buf))
			break ;
	}
}
