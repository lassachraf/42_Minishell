/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 18:26:57 by alassiqu          #+#    #+#             */
/*   Updated: 2024/06/30 09:56:17 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	nb_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

int	nb_options(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (!ft_strncmp(args[i], "-", 1))
			return (0);
		i++;
	}
	return (1);
}

int	builtins_exec_check(char **args)
{
	if (!ft_strcmp(args[0], "env") && (nb_args(args) > 1 || !nb_options(args)))
	{
		ft_putstr_fd(RED, 2);
		ft_putstr_fd("badashell$ : env: can't run it with ", 2);
		if (nb_args(args) > 1 && !nb_options(args))
			ft_putstr_fd("args and options.\n" RESET, 2);
		else if (nb_args(args) > 1)
			ft_putstr_fd("args.\n" RESET, 2);
		else if (nb_options(args))
			ft_putstr_fd("options.\n" RESET, 2);
		set_env_var(g_minishell->our_env, "?", "1", 0);
		return (1);
	}
	if ((!ft_strcmp(args[0], "export") || !ft_strcmp(args[0], "exit")
		|| !ft_strcmp(args[0], "unset")) && nb_options(args) == 0)
	{
		ft_putstr_fd(RED "badashell$ : ", 2);
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd(": can't run it with options.\n" RESET, 2);
		set_env_var(g_minishell->our_env, "?", "1", 0);
		return (1);
	}
	return (0);
}

int	check_cd_and_exit(t_minishell *mini, char **args, int flag)
{
	if (!flag)
	{
		if (nb_args(args) > 2)
		{
			return (0);
			print_errors("cd : too many arguments.");
		}
		else
			ft_cd(mini, args[1]);
	}
	else
	{
		if (nb_args(args) > 2)
		{
			print_errors("exit : too many arguments.");
			return (0);
		}
		else
			ft_exit(args[1]);
	}
	return (1);
}

void	execute_builtins(t_minishell *mini, char **args)
{
	for (int i = 0; i < nb_args(args); i++)
        printf("** Arg %d => %s **\n", i, args[i]);
	if (builtins_exec_check(args))
		return ;
	if (ft_strcmp(args[0], "echo") == 0)
		ft_echo(args);
	else if (!ft_strcmp(args[0], "cd"))
		check_cd_and_exit(mini, args, 0);
	else if (!ft_strcmp(args[0], "exit"))
		check_cd_and_exit(mini, args, 1);
	else if (ft_strcmp(args[0], "env") == 0)
		ft_env(mini->our_env);
	else if (ft_strcmp(args[0], "pwd") == 0)
		ft_pwd(mini->our_env);
	else if (ft_strcmp(args[0], "export") == 0)
		ft_export(args, nb_args(args));
	else if (ft_strcmp(args[0], "unset") == 0)
		ft_unset();
}

bool	ft_is_builtin(char *arg)
{
	if (!arg)
		return (false);
	if (!ft_strcmp(arg, "echo") || !ft_strcmp(arg, "cd") || !ft_strcmp(arg,
			"exit") || !ft_strcmp(arg, "pwd") || !ft_strcmp(arg, "export")
		|| !ft_strcmp(arg, "unset") || !ft_strcmp(arg, "env"))
		return (true);
	return (false);
}
