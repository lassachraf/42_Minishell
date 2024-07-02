/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 16:30:50 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/02 16:39:35 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	set_as_visible(t_env *env, char *var)
{
	while (env && ft_strncmp(env->key, var, ft_strlen(env->key)))
		env = env->next;
	if (!env)
		return ;
	env->visible = true;
}

void	set_as_invisible(t_env *env, char *var)
{
	while (env && ft_strncmp(env->key, var, ft_strlen(env->key)))
		env = env->next;
	if (!env)
		return ;
	env->visible = false;
}

void	set_as_exported(t_env *env, char *var)
{
	while (env && ft_strncmp(env->key, var, ft_strlen(env->key)))
		env = env->next;
	if (!env)
		return ;
	env->export = true;
}

void	set_as_unexported(t_env *env, char *var)
{
	while (env && ft_strncmp(env->key, var, ft_strlen(env->key)))
		env = env->next;
	if (!env)
		return ;
	env->export = false;
}
