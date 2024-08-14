/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 10:48:03 by baouragh          #+#    #+#             */
/*   Updated: 2024/08/14 13:09:30 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	add_list_into_list(t_list **lst, t_list *new)
{
	t_list	*save_next;

	if (!lst || !*lst || !new)
		return ;
	save_next = (*lst)->next;
	(*lst)->content = new->content;
	(*lst)->next = new->next;
	ft_lstlast(*lst)->next = save_next;
}

t_list	*creat_list(char **split, bool avoid)
{
	t_list	*lst;
	char	*tmp;
	t_list	*temp;
	int		i;

	i = 0;
	lst = NULL;
	while (split[i])
	{
		tmp = ft_strdup(split[i]);
		gc_add(g_minishell, tmp);
		temp = ft_lstnew(tmp);
		gc_add(g_minishell, temp);
		ft_lstadd_back(&lst, temp);
		i++;
	}
	if (avoid)
		free_double(split);
	return (lst);
}

void	expand_list(t_list *cmds)
{
	t_list	*list;
	bool	avoid;

	list = NULL;
	avoid = 1;
	if (!cmds)
		return ;
	while (cmds)
	{
		check_for_export(cmds->content, &avoid);
		if (cmds->content)
		{
			if (ft_strchr((char *)cmds->content, '$') && cmds->wd_expand)
				dollar_functionality(&cmds, (char **)&cmds->content, avoid);
			else if (ft_strchr((char *)cmds->content, '*') && cmds->wd_expand)
			{
				list = asterisk_functionality((char *)cmds->content);
				add_list_into_list(&cmds, list);
			}
		}
		cmds = cmds->next;
	}
}

void	execute_cmd(t_node *node)
{
	if (!node)
		return ;
	expand_list(node->data.cmd);
	remove_null(&node);
	if (!node->data.cmd)
		return (set_env_var(g_minishell->our_env, "_", ""));
	set_env_var(g_minishell->our_env, "_",
		(char *)ft_lstlast(node->data.cmd)->content);
	if (ft_is_builtin(node->data.cmd->content))
		execute_builtins(g_minishell, list_to_argv(node->data.cmd), 1);
	else
	{
		g_minishell->last_child = fork();
		if (!g_minishell->last_child)
		{
			signal(SIGQUIT, SIG_DFL);
			do_cmd(node, 1);
			exit(0);
		}
	}
}

void	execute_redires(t_list *red_list)
{
	int	old_stdin;
	int	old_stdout;

	old_stdin = dup(0);
	old_stdout = dup(1);
	if (!open_and_set(red_list))
		return ;
	input_to_dup(red_list);
	output_to_dup(red_list);
	run_doc_cmd(red_list);
	dup_2(old_stdin, 0);
	dup_2(old_stdout, 1);
}
