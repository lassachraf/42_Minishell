/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:20:22 by baouragh          #+#    #+#             */
/*   Updated: 2024/07/19 20:46:23 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	wait_and_get(void)
{
	int	fail;
	int x;
	char *exit;


	fail = -1;
	fail = wait(&x);
	if(g_minishell->exit_s == 130)
	{
		exit = ft_itoa(g_minishell->exit_s);
		set_env_var(g_minishell->our_env, "?", exit);
		return (free(exit) ,-1);
	}
	if (WIFEXITED(x))
		g_minishell->exit_s = WEXITSTATUS(x);
	exit = ft_itoa(g_minishell->exit_s);
	set_env_var(g_minishell->our_env, "?", exit);
	return (free(exit) ,fail);
}

void	do_cmd(t_node *ast)
{
	int		id;
	char	**cmd;
	char	**env;

	if (!ast)
		return ;
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
			return ;
		id = check_cmd(*cmd, env);
		if (!id)
			call_execev(env, *cmd, cmd);
	}
	exit(id);
}

void	do_pipe(t_node *cmd, int mode, int *pfd)
{
	int	id;
	t_list *lst;
	t_list *asterisk;

	id = fork();
	if (id < 0)
	{
		print_err("error occuerd with fork!", NULL);
		return ;
	}
	if (id == 0)
	{
		fd_duper(pfd, mode);
		lst = cmd->data.cmd;
		while(lst)
		{
			if (ft_strchr((char*)lst->content, '$'))
				here_doc_expanding((char**)&lst->content);
			else if(ft_strchr((char*)lst->content, '*'))
			{
				asterisk = asterisk_functionality((char*)lst->content);
				add_list_into_list(&lst, asterisk);
			}
			lst = lst->next;
		}
		set_null_as_true(&cmd);
		if(!cmd->data.cmd)
			exit(0);
		do_cmd(cmd);
	}
	else
	{
		close(pfd[1]);
		dup2(pfd[0], 0);
		if (mode)
			wait_and_get();
	}
}

int	execute_docs(t_list *red_list)
{
	if (do_here_docs(red_list, g_minishell->docs) == 0)
		return (0);
	return (1);
}

void	unlink_docs(int docs)
{
	char	*name;
	char	*join;

	if (!docs)
		return ;
	while (docs >= 0)
	{
		join = ft_itoa(docs);
		name = ft_strjoin(PATH, join);
		unlink(name);
		free(join);
		free(name);
		docs--;
	}
}
