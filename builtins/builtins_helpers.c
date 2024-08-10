/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 14:50:59 by alassiqu          #+#    #+#             */
/*   Updated: 2024/08/09 09:23:29 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	ft_is_builtin(char *arg)
{
	if (!arg)
		return (false);
	if (!ft_strcmp(arg, "echo") || !ft_strcmp(arg, "cd") || !ft_strcmp(arg,
			"exit") || !ft_strcmp(arg, "pwd") || !ft_strcmp(arg, "export")
		|| !ft_strcmp(arg, "unset") || !ft_strcmp(arg, "env"))
		return (true);
	return (false);
}

void	set_new_node(t_env **head, t_env **current, t_env *env)
{
	t_env	*new_node;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->key = ft_strdup(env->key);
	if (!env->value)
		new_node->value = NULL;
	else
		new_node->value = ft_strdup(env->value);
	new_node->visible = env->visible;
	new_node->export = env->export;
	new_node->next = NULL;
	if (!*head)
		(*head) = new_node;
	else
		(*current)->next = new_node;
	(*current) = new_node;
}

t_env	*new_dup(t_env *env)
{
	t_env	*head;
	t_env	*current;

	head = NULL;
	current = NULL;
	while (env)
	{
		if (!env->export)
		{
			env = env->next;
			continue ;
		}
		set_new_node(&head, &current, env);
		env = env->next;
	}
	return (head);
}

void	free_split(char **s)
{
	int	i;

	i = 0;
	while (s[i])
		free(s[i++]);
	free(s);
}

void	joining_words(char **split)
{
	char	*temp;
	char	*new;

	new = NULL;
	set_as_visible(g_minishell->our_env, split[0]);
	set_as_exported(g_minishell->our_env, split[0]);
	temp = get_env_var(g_minishell->our_env, split[0]);
	if (split[1])
		new = ft_strjoin(temp, split[1]);
	else
		return ;
	gc_add(g_minishell, new);
	set_env_var(g_minishell->our_env, split[0], new);
}
