/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 14:26:57 by alassiqu          #+#    #+#             */
/*   Updated: 2024/06/30 09:57:43 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_env(t_env *env)
{
	set_env_var(env, "_", "/usr/bin/env", 1);
	while (env)
	{
		if (env->visible)
			printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
}
