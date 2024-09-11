/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 14:23:51 by alassiqu          #+#    #+#             */
/*   Updated: 2024/09/11 11:00:38 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_pwd(t_env *env)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	gc_add(g_minishell, cwd);
	printf("%s\n", cwd);
	g_minishell->exit_s = 0;
	return ;
}
