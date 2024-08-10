/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 14:26:57 by alassiqu          #+#    #+#             */
/*   Updated: 2024/08/10 18:27:50 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_env(t_env *env)
{
	set_env_var(env, "_", "/usr/bin/env");
	while (env)
	{
		if (env->visible)
		{
			printf("%s=", env->key);
			if (env->value)
				printf("%s\n", env->value);
			else
				printf("\n");
		}
		env = env->next;
	}
	g_minishell->exit_s = 0;
}
