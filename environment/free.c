/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 15:37:10 by alassiqu          #+#    #+#             */
/*   Updated: 2024/06/27 18:28:46 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
