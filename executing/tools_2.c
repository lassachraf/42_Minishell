/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:20:22 by baouragh          #+#    #+#             */
/*   Updated: 2024/07/30 17:46:39 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	wait_and_get(void)
{
	int		fail;
	int		x;
	char	*exit;

	fail = -1;
	fail = wait(&x);
	// if (g_minishell->exit_s == 130)
	// {
	// 	exit = ft_itoa(g_minishell->exit_s);
	// 	set_env_var(g_minishell->our_env, "?", exit);
	// 	return (free(exit), -1);
	// }
	if (WIFEXITED(x))
		g_minishell->exit_s = WEXITSTATUS(x);
	exit = ft_itoa(g_minishell->exit_s);
	set_env_var(g_minishell->our_env, "?", exit);
	return (free(exit), fail);
}

void	do_cmd(t_node *ast)
{
	int		id;
	char	**cmd;
	char	**env;

	if (!ast)
		exit(0);
	id = 0;
	if (ft_is_builtin(ast->data.cmd->content))
		execute_builtins(g_minishell, list_to_argv(ast->data.cmd));
	else
	{
		cmd = list_to_argv(ast->data.cmd);
		if (!cmd)
			exit(0);
		env = env_to_envp(g_minishell->our_env);
		if (!env)
			exit(0);
		id = check_cmd(*cmd);
		if (!id)
			call_execev(env, *cmd, cmd);
	}
	exit(id);
}

void	do_pipe(t_node *cmd, int mode, int *pfd)
{
	int		id;
	t_list	*cmd_lst;
	t_list	*list;

	id = fork();
	if (id < 0)
	{
		print_err("error occuerd with fork!", NULL);
		return ;
	}
	if (id == 0)
	{
		fd_duper(pfd, mode);
		cmd_lst = cmd->data.cmd;
		while (cmd_lst)
		{
			if (ft_strchr((char *)cmd_lst->content, '$') && cmd_lst->wd_expand)
			{
				list = dollar_functionality((char **)&cmd_lst->content);
				add_list_into_list(&cmd_lst, list);
			}
			else if (ft_strchr((char *)cmd_lst->content, '*'))
			{
				list = asterisk_functionality((char *)cmd_lst->content);
				add_list_into_list(&cmd_lst, list);
			}
			cmd_lst = cmd_lst->next;
		}
		remove_null(&cmd);
		if (!cmd->data.cmd)
			exit(0);
		do_cmd(cmd);
		exit(0);
	}
	else
	{
		close(pfd[1]);
		dup2(pfd[0], 0);
		// if (mode)
		// 	wait_and_get();
	}
}

int	execute_docs(t_list *red_list)
{
	if (do_here_docs(red_list) == 0)
		return (0);
	return (1);
}

void	unlink_docs(int docs)
{
	char	*name;
	char	*path;
	char	*join;

	if (!docs)
		return ;
	while (docs >= 0)
	{
		join = ft_itoa(docs);
		path = build_file_name(join);
		name = ft_strjoin(PATH, path);
		unlink(name);
		free(join);
		free(path);
		free(name);
		docs--;
	}
}
