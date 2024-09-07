/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 09:20:20 by alassiqu          #+#    #+#             */
/*   Updated: 2024/09/07 14:59:07 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_tree(t_node **tree)
{
	t_redir	*new;
	t_list	*tmp;

	new = NULL;
	while ((*tree)->data.redir)
	{
		new = (*tree)->data.redir->content;
		tmp = (*tree)->data.redir->next;
		if (new->cmd)
		{
			ft_lstclear(&new->cmd, free);
			free(new->cmd);
		}
		free(new->file);
		free((*tree)->data.redir);
		free(new);
		(*tree)->data.redir = tmp;
	}
	free((*tree));
}

void	clear_ast(t_node *tree)
{
	if (!tree)
		return ;
	if (tree->type == STRING_NODE)
	{
		ft_lstclear(&tree->data.cmd, free);
		free(tree);
	}
	else if (tree->type == PAIR_NODE)
	{
		clear_ast(tree->data.pair.left);
		clear_ast(tree->data.pair.right);
	}
	else if (tree->type == REDIR_NODE)
	{
		free_tree(&tree);
	}
}

char	*ft_malloc(t_minishell *mini, size_t size)
{
	char	*memory;

	memory = malloc(size);
	if (!memory)
		return (perror("Malloc failed!"), NULL);
	gc_add(mini, memory);
	return (memory);
}

void	cleanup_minishell(void)
{
	clear_env(g_minishell->our_env);
	close(g_minishell->stdin);
	close(g_minishell->stdout);
	if (g_minishell->her_pfd)
	{
		close(g_minishell->her_pfd[0]);
		close(g_minishell->her_pfd[1]);
	}
	gc_free_all(g_minishell);
	free(g_minishell);
}
