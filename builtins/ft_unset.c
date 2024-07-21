/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 19:04:16 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/21 20:38:09 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_unset(char *key)
{
	t_env	*env;

	env = g_minishell->our_env;
	if (ft_strncmp(key, "_", ft_strlen(key)))
		delete_env_var(&env, key);
}
