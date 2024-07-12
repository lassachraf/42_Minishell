/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normal_expand.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 16:19:01 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/12 16:23:52 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void    directories()
{
    
}

void    files()
{
    DIR		        *dir;
    struct dirent   *entry;

    dir = opendir(".");
    entry = readdir(dir);
    while (entry)
    {
        
        entry = readdir(dir);
    }
}

void    normal_expand(t_token **tokens, t_token *curr)
{
    
}