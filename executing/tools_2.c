/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:20:22 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/09 09:55:31 by alassiqu         ###   ########.fr       */
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
	return (free(exit), fail);
}

void	do_cmd(t_node *ast, bool print) // 
{
	char	**cmd;
	char	**env;
	int		i;

	i = 0;
	if (!ast)
		ft_exit(NULL, 0);
	if (ft_is_builtin(ast->data.cmd->content))
		execute_builtins(g_minishell, list_to_argv(ast->data.cmd), print);
	else
	{
		cmd = list_to_argv(ast->data.cmd);
		if (!cmd)
			ft_exit(NULL, 0);
		env = env_to_envp(g_minishell->our_env);
		if (!env)
			ft_exit(NULL, 0);
		g_minishell->last_child = check_cmd(*cmd);
		if (!g_minishell->last_child)
			call_execev(env, *cmd, cmd);
	}
	i = g_minishell->last_child;
	cleanup_minishell();
	exit(i);
}

void	do_pipe(t_node *cmd, int *pfd, int *fd_io)
{
	g_minishell->last_child = fork();
	if (g_minishell->last_child < 0)
	{
		print_err("error occuerd with fork!", NULL);
		return ;
	}
	if (!g_minishell->last_child)
	{
		signal(SIGQUIT, SIG_DFL);
		clean_fds(g_minishell->ast);
		close_fds(pfd, fd_io);
		expand_list(cmd->data.cmd);
		remove_null(&cmd);
		if (!cmd->data.cmd)
			ft_exit(NULL, 0);
		do_cmd(cmd, 0);
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

	if (!docs)
		return ;
	while (docs > 0)
	{
		path = get_filename(docs);
		name = ft_strjoin(PATH, path);
		if (unlink(name) == -1)
			perror("unlink failed");
		free(name);
		docs--;
	}
}
