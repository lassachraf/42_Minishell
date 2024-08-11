/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:20:22 by baouragh          #+#    #+#             */
/*   Updated: 2024/08/11 17:21:44 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	wait_and_get(void)
{
	int		fail;
	int		x;
	char	*exit;

	x = -1;
	fail = -1;
	fail = wait(&x);
	if (x != -1 && WIFEXITED(x))
		g_minishell->exit_s = WEXITSTATUS(x);
	exit = ft_itoa(g_minishell->exit_s);
	set_env_var(g_minishell->our_env, "?", exit);
	return (free(exit), fail);
}

void	do_cmd(t_node *ast, bool print)
{
	char	**cmd;
	char	**env;

	if (!ast)
		exit(0);
	if (ft_is_builtin(ast->data.cmd->content))
		execute_builtins(g_minishell, list_to_argv(ast->data.cmd), print);
	else
	{
		cmd = list_to_argv(ast->data.cmd);
		if (!cmd)
			exit(0);
		env = env_to_envp(g_minishell->our_env);
		if (!env)
			exit(0);
		g_minishell->last_child = check_cmd(*cmd);
		if (!g_minishell->last_child)
			call_execev(env, *cmd, cmd);
	}
	exit(g_minishell->last_child);
}

void	do_pipe(t_node *cmd, int mode, int *pfd)
{
	g_minishell->last_child = fork();
	if (g_minishell->last_child < 0)
	{
		print_err("error occuerd with fork!", NULL);
		return ;
	}
	if (g_minishell->last_child == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		fd_duper(pfd, mode);
		expand_list(cmd->data.cmd);
		remove_null(&cmd);
		if (!cmd->data.cmd)
			exit(0);
		do_cmd(cmd, 0);
		exit(0);
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
	while (docs > 0)
	{
		join = ft_itoa(docs);
		path = build_file_name(join);
		name = ft_strjoin(PATH, path);
		if (unlink(name) == -1)
			perror("unlink failed");
		free(join);
		free(path);
		free(name);
		docs--;
	}
}
