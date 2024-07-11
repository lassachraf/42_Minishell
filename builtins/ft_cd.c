/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 18:22:14 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/11 21:51:02 by alassiqu         ###   ########.fr       */
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

	set_env_var(mini->our_env, "OLDPWD", get_env_var(mini->our_env, "PWD"));
	home = get_env_var(mini->our_env, "HOME");
	if (!home)
		return (print_errors("cd: HOME not set"), 1);
	if (!chdir(home))
		return (set_env_var(mini->our_env, "PWD", home), 0);
	return (1);
}

char	*custome_path(char *path)
{
	char	*new_path;
	char	*home;
	int		i;
	int		j;

	home = get_env_var(g_minishell->our_env, "HOME");
	new_path = malloc(sizeof(char) * (ft_strlen(home) + ft_strlen(path)));
	if (!new_path)
		print_errors("Allocation failed !");
	if (!ft_strncmp(path, "~", 1))
	{
		i = -1;
		while (home[++i])
			new_path[i] = home[i];
		j = 0;
		while (path[++j])
			new_path[i++] = path[j];
		new_path[i] = 0;
	}
	gc_add(g_minishell, new_path);
	return (new_path);
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
	set_env_var(mini->our_env, "OLDPWD", get_env_var(mini->our_env, "PWD"));
	cwd = getcwd(NULL, 0);
	gc_add(g_minishell, cwd);
	if (!cwd)
	{
		print_errors("getcwd: unable to determine current directory.");
		return ;
	}
	set_env_var(mini->our_env, "PWD", cwd);
}
