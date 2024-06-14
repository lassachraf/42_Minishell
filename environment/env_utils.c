/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 14:46:08 by alassiqu          #+#    #+#             */
/*   Updated: 2024/06/14 13:03:57 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	add_env_var(t_env *env, char *key, char *value, bool visible)
{
	t_env	*new_node;
	t_env	*current;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	new_node->visible = visible;
	new_node->next = NULL;
	if (!env)
		env = new_node;
	else
	{
		current = env;
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
}

int	ft_key_length(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '=')
		i++;
	return (i);
}

void	set_env_var(t_env *env, char *var, char *new)
{
	while (env && ft_strncmp(env->key, var, ft_strlen(env->key)))
		env = env->next;
	if (!env)
		return ;
	free(env->value);
	env->value = ft_strdup(new);
}

char	*get_env_var(t_env *env, char *var)
{
	if (!ft_strncmp(var, "\0", 1))
		return (NULL);
	while (env && ft_strncmp(env->key, var, ft_strlen(env->key)))
		env = env->next;
	if (env && ft_strlen(env->key) == ft_strlen(var))
		return (env->value);
	else
		return (NULL);
}

t_env	*dup_env(char **env)
{
	t_env	*head;
	t_env	*current;
	t_env	*new_node;
	int		i;

	head = NULL;
	current = NULL;
	i = 0;
	while (env[i])
	{
		new_node = (t_env *)malloc(sizeof(t_env));
		if (!new_node)
			return (NULL);
		new_node->key = ft_substr(env[i], 0, ft_key_length(env[i]));
		new_node->value = ft_strdup(getenv(new_node->key));
		new_node->visible = true;
		new_node->next = NULL;
		if (!head)
			head = new_node;
		else
			current->next = new_node;
		current = new_node;
		i++;
	}
	return (head);
}
