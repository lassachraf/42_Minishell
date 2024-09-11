/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 10:48:03 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/09 16:19:48 by baouragh         ###   ########.fr       */
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

t_list	*creat_list(char **split, bool free)
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
	if (free)
		free_double(split);
	temp = lst;
	return (lst);
}

void	expand_list(t_list *cmds)
{
	t_list	*list;
	bool	avoid;
	int		export;

	list = NULL;
	export = 0;
	avoid = 1;
	if (!cmds)
		return ;
	while (cmds)
	{
		if (cmds->content && !ft_strcmp(cmds->content, "export")
			&& ft_lstsize(cmds) > 1)
			check_for_export(&cmds, &avoid, &export);
		else if (cmds->content)
		{
			if (ft_strchr((char *)cmds->content, '$') && cmds->is_quoted <= 1
				&& !export)
				dollar_functionality(&cmds, (char **)&cmds->content,
					cmds->avoid_spaces);
			else if (ft_strchr((char *)cmds->content, '*') && cmds->wd_expand)
				asterisk_functionality_2(&cmds, (char *)cmds->content);
		}
		cmds = cmds->next;
	}
}

void	execute_cmd(t_node *node, int *pfd, int *fd_io)
{
	if (!node)
		return ;
	clean_fds(g_minishell->ast);
	close_fds(pfd, fd_io);
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
			clean_fds(g_minishell->ast);
			close_fds(pfd, fd_io);
			signal(SIGQUIT, SIG_DFL);
			do_cmd(node, 1);
		}
	}
}

void	execute_redires(t_list *red_list, int *pfd, int *fd_io)
{
	int	fd_ios[4];

	if (!open_and_set(red_list))
	{
		close_fds(pfd, fd_io);
		return ;
	}
	if (fd_io)
	{
		fd_ios[2] = fd_io[0];
		fd_ios[3] = fd_io[1];
		g_minishell->size = 4;
	}
	fd_ios[0] = dup(0);
	if (fd_ios[0] < 0)
		return ;
	fd_ios[1] = dup(1);
	if (fd_ios[1] < 0)
		return (close(fd_ios[0]), (void)0);
	input_to_dup(red_list);
	output_to_dup(red_list);
	run_doc_cmd(red_list, pfd, fd_ios);
	dup_2(fd_ios[0], 0);
	dup_2(fd_ios[1], 1);
	close_fds(pfd, fd_io);
}
