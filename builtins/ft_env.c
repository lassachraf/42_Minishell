/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 14:26:57 by alassiqu          #+#    #+#             */
/*   Updated: 2024/08/03 19:38:00 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_env(t_env *env)
{
	set_env_var(env, "_", "/usr/bin/env");
	while (env)
	{
		if (env->visible)
			printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
	g_minishell->exit_s = 0;
}
