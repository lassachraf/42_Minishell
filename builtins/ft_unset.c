/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 19:04:16 by alassiqu          #+#    #+#             */
/*   Updated: 2024/08/02 17:41:49 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_unset(char *key)
{
	t_env	*env;

	env = g_minishell->our_env;
	if (ft_strncmp(key, "_", ft_strlen(key)))
		delete_env_var(&env, key);
	g_minishell->exit_s = 0;
}
