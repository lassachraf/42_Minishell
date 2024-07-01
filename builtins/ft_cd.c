/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 18:22:14 by alassiqu          #+#    #+#             */
/*   Updated: 2024/06/30 09:57:27 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	cd_error(char *path)
{
	ft_putstr_fd(RED "badashell$: cd: `", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd("`: No such file or directory\n" RESET, 2);
}

int	cd_home(t_minishell *mini)
{
	char	*home;

	set_env_var(mini->our_env, "OLDPWD", get_env_var(mini->our_env, "PWD"), 1);
	home = get_env_var(mini->our_env, "HOME");
	if (!home)
		return (print_errors("badashell$: cd: HOME not set"), 1);
	if (!chdir(home))
		return (set_env_var(mini->our_env, "PWD", home, 1), 0);
	return (1);
}

void	ft_cd(t_minishell *mini, char *path)
{
	char	*cwd;

	if (!path)
	{
		cd_home(mini);
		return ;
	}
	if (chdir(path))
	{
		cd_error(path);
		return ;
	}
	set_env_var(mini->our_env, "OLDPWD", get_env_var(mini->our_env, "PWD"), 1);
	cwd = getcwd(NULL, 0);
	gc_add(g_minishell, cwd);
	if (!cwd)
	{
		print_errors("getcwd: unable to determine current directory.");
		return ;
	}
	set_env_var(mini->our_env, "PWD", cwd, 1);
}
