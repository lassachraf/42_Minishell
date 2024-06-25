/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 11:30:18 by alassiqu          #+#    #+#             */
/*   Updated: 2024/06/25 09:49:04 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	clear_ast(t_node *tree)
{
    t_redir *new;
    t_list *tmp;

    new = NULL;
    if (!tree) return;
    if (tree->type == STRING_NODE)
    {
        ft_lstclear(&tree->data.cmd, free);
        free(tree);
    }
	else if(tree->type == PAIR_NODE)
	{
        clear_ast(tree->data.pair.left);
        clear_ast(tree->data.pair.right);
    }
    else if (tree->type == REDIR_NODE)
    {
        while(tree->data.redir)
        {
            new = tree->data.redir->content;
            tmp = tree->data.redir->next;
            if (new->cmd)
            {
                ft_lstclear(&new->cmd, free);
                free(new->cmd);    
            }
            free(new->file);
            free(tree->data.redir);
            free(new);
            tree->data.redir = tmp;
        }
        free(tree);
    }
    else if(tree->type == ERROR_NODE)
        free(tree);
}

void	cleanup(void)
{
	gc_free_all(g_minishell);
	clear_token(&g_minishell->tokens);
	free(g_minishell->line);
}

void	cleanup_minishell(void)
{
	clear_env();
	free(g_minishell);
}
