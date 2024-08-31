/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 10:48:03 by baouragh          #+#    #+#             */
/*   Updated: 2024/08/31 17:04:17 by alassiqu         ###   ########.fr       */
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
	temp = lst;
	return (lst);
}

// int	len_of_contents(t_list *list)
// {
// 	int	len;

// 	len = 0;
// 	while (list)
// 	{
// 		if (list->content)
// 			len += ft_strlen(list->content);
// 		list = list->next;
// 	}
// 	return (len);
// }

// char	*list_to_charp(t_list *list)
// {
// 	char	*charp;
// 	int		size;
// 	int		i;
// 	int		len;

// 	i = 0;
// 	if (!list || !list->content)
// 		return (NULL);
// 	list = list->next;
// 	size = len_of_contents(list);
// 	charp = malloc(sizeof(char) * (size + 1));
// 	if (!charp)
// 		return (gc_free_all(g_minishell), NULL);
// 	gc_add(g_minishell, charp);
// 	while (i < size)
// 	{
// 		len = ft_strlen(list->content); 
// 		ft_memmove(charp + i, list->content, len);
// 		i += len;
// 		list = list->next;
// 	}
// 	charp[i] = '\0';
// 	return (charp);
// }

void	expand_list(t_list *cmds) // export $a=$b ok=a gy=$l > 1 
{
	t_list	*list;
	bool	avoid;
	bool	export;

	list = NULL;
	export = 0;
	avoid = 1;
	if (!cmds)
		return ;
	while (cmds)
	{
		if (cmds->content && !ft_strcmp(cmds->content, "export") && ft_lstsize(cmds) > 1)
		{
			check_for_export(&cmds, &avoid);
			export = 1;
		}
		else if (cmds->content)
		{
			if (ft_strchr((char *)cmds->content, '$') && cmds->wd_expand && !export)
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

void	execute_cmd(t_node *node, int *pfd)
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
			if (pfd)
				fd_closer(pfd);
			signal(SIGQUIT, SIG_DFL);
			do_cmd(node, 1);
			exit(0);
		}
	}
}

void	execute_redires(t_list *red_list, int *pfd)
{
	int	old_stdin;
	int	old_stdout;

	old_stdin = dup(0);
	if (old_stdin < 0)
		return ;
	old_stdout = dup(1);
	if (old_stdout < 0)
	{
		close(old_stdin);
		return ;
	}
	if (!open_and_set(red_list))
	{
		close(old_stdin);
		close(old_stdout);
		return ;
	}
	input_to_dup(red_list);
	output_to_dup(red_list);
	run_doc_cmd(red_list, pfd);
	dup_2(old_stdin, 0);
	dup_2(old_stdout, 1);
}
