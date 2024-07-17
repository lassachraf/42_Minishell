/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:20:22 by baouragh          #+#    #+#             */
/*   Updated: 2024/07/17 10:06:51 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	wait_and_get(void)
{
	int	fail;

	// char	*exit;
	fail = -1;
	fail = wait(&g_minishell->exit_s);
	// printf("%d\n",g_minishell->exit_s );
	if (WIFEXITED(g_minishell->exit_s))
		g_minishell->exit_s = WEXITSTATUS(g_minishell->exit_s);
	// printf(">>>>>>>>>>>> %d\n",g_minishell->exit_s);
	// exit = ft_itoa(g_minishell->exit_s);
	// if(!exit)
	// 	return(print_errors("ERROR WITH FT_ITOA\n"), fail);
	// set_env_var(g_minishell->our_env, "?", exit);
	// free(exit);
	return (fail);
}

void	do_cmd(t_node *ast)
{
	int		id;
	char	**cmd;
	char	**env;

	id = 0;
	if (!ast)
		return ;
	else if (ft_is_builtin(ast->data.cmd->content))
		execute_builtins(g_minishell, list_to_argv(ast->data.cmd));
	else
	{
		cmd = list_to_argv(ast->data.cmd);
		if (!cmd)
			return ;
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

	id = fork();
	if (id < 0)
	{
		print_err("error occuerd with fork!", NULL);
		return ;
	}
	if (id == 0)
	{
		fd_duper(pfd, mode);
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
