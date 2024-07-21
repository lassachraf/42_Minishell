/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 15:37:10 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/18 10:48:09 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	set_env_var(t_env *env, char *var, char *new)
{
	while (env && ft_strncmp(env->key, var, ft_strlen(env->key)))
		env = env->next;
	if (!env)
		return ;
	free(env->value);
	if (!new)
		env->value = NULL;
	else if (!ft_strcmp(new, "\0"))
		env->value = ft_strdup("");
	else
		env->value = ft_strdup(new);
}

char	*get_env_var(t_env *env, char *var)
{
	if (!var)
		return (NULL);
	while (env && ft_strncmp(env->key, var, ft_strlen(env->key)))
		env = env->next;
	if (env && ft_strlen(env->key) == ft_strlen(var))
	{
		if (env->value)
			return (env->value);
		else
			return ("");
	}
	else
		return (NULL);
}

void	delete_env_var(t_env **env, char *key)
{
	t_env	*current;
	t_env	*previous;

	if (!env || !*env)
		return ;
	current = *env;
	previous = NULL;
	while (current && ft_strncmp(current->key, key, ft_strlen(key)) != 0)
	{
		previous = current;
		current = current->next;
	}
	if (!current)
		return ;
	if (!previous)
		*env = current->next;
	else
		previous->next = current->next;
	free(current->key);
	free(current->value);
	free(current);
}

void	clear_env(t_env *env)
{
	t_env	*current;
	t_env	*next;

	current = env;
	while (current)
	{
		next = current->next;
		free(current->value);
		free(current->key);
		free(current);
		current = next;
	}
	env = NULL;
}
