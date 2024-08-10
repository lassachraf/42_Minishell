/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 10:48:03 by baouragh          #+#    #+#             */
/*   Updated: 2024/08/09 09:24:44 by alassiqu         ###   ########.fr       */
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

void	expand_list(t_list *cmd_lst)
{
	t_list	*list;
	bool	avoid;

	list = NULL;
	avoid = 1;
	if (!cmd_lst)
		return ;
	while (cmd_lst)
	{
		check_for_export(cmd_lst->content, &avoid);
		if (cmd_lst->content)
		{
			if (ft_strchr((char *)cmd_lst->content, '$') && cmd_lst->wd_expand)
			{
				list = dollar_functionality((char **)&cmd_lst->content, avoid);
				add_list_into_list(&cmd_lst, list);
			}
			else if (ft_strchr((char *)cmd_lst->content, '*'))
			{
				list = asterisk_functionality((char *)cmd_lst->content);
				add_list_into_list(&cmd_lst, list);
			}
		}
		cmd_lst = cmd_lst->next;
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
