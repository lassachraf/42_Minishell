/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:14:45 by baouragh          #+#    #+#             */
/*   Updated: 2024/07/09 11:56:21 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int open_redir(t_redir *redir)
{
	redir->fd = open(redir->file,redir->mode, 0644); // mode : O_R , O_RW
    if(redir->fd < 0)
    {
        if(access(redir->file, F_OK) == -1)
            print_err("badashell: no such file or directory: ",redir->file);
		else
            print_err("badashell: permission denied: ",redir->file);
        g_minishell->exit_s = 1;
        return (0);
    }
    return (1);
}

int open_and_set(t_list *red_list)
{
    t_redir *new ;
	while(red_list) // linked list of reds
    {
		new = red_list->content;
        // printf("REDIR NODE , name: '%s'\n",new->file);
		if(new->type != LL_REDIR)
			if(!open_redir(new))
                return (0);
        red_list = red_list->next;
    }
    return (1);
}
