/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 09:20:20 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/12 22:55:31 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	clear_ast(t_node *tree)
{
	t_redir	*new;
	t_list	*tmp;

	new = NULL;
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
		while (tree->data.redir)
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
	else if (tree->type == ERROR_NODE)
		free(tree);
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

void	cleanup(void)
{
	gc_free_all(g_minishell);
	clear_token(&g_minishell->tokens);
	free(g_minishell->line);
}

void	cleanup_minishell(void)
{
	gc_free_all(g_minishell);
	clear_env(g_minishell->our_env);
	free(g_minishell);
}
