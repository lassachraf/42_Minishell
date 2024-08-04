/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 14:46:08 by alassiqu          #+#    #+#             */
/*   Updated: 2024/08/03 22:26:32 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	add_env_var(t_env *env, char *key, char *value)
{
	t_env	*new_node;
	t_env	*current;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->key = ft_strdup(key);
	if (!value)
		new_node->value = NULL;
	else
		new_node->value = ft_strdup(value);
	new_node->visible = true;
	new_node->export = true;
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

t_env	*dup_env(char **env)
{
	t_env	*head;
	t_env	*current;
	t_env	*new_node;
	int		i;

	head = NULL;
	current = NULL;
	i = -1;
	while (env[++i])
	{
		new_node = (t_env *)malloc(sizeof(t_env));
		if (!new_node)
			return (NULL);
		new_node->key = ft_substr(env[i], 0, ft_key_length(env[i]));
		new_node->value = ft_strdup(getenv(new_node->key));
		new_node->visible = true;
		new_node->export = true;
		new_node->next = NULL;
		if (!head)
			head = new_node;
		else
			current->next = new_node;
		current = new_node;
	}
	return (head);
}

void	add_4_element(t_env *head)
{
	add_env_var(head, "SHLVL", "1");
	add_env_var(head, "_", "/usr/bin/env");
	add_env_var(head, "OLDPWD", NULL);
	set_as_invisible(head, "OLDPWD");
}

t_env	*special_dup_env(void)
{
	t_env	*head;
	t_env	*pwd;
	t_env	*shlvl;
	t_env	*underscore;
	char	*cwd;

	head = NULL;
	cwd = getcwd(NULL, 0);
	gc_add(g_minishell, cwd);
	pwd = (t_env *)malloc(sizeof(t_env));
	shlvl = (t_env *)malloc(sizeof(t_env));
	underscore = (t_env *)malloc(sizeof(t_env));
	gc_add(g_minishell, shlvl);
	gc_add(g_minishell, underscore);
	if (!pwd || !shlvl || !underscore)
		return (NULL);
	pwd->key = ft_strdup("PWD");
	pwd->value = ft_strdup(cwd);
	pwd->visible = true;
	pwd->export = true;
	pwd->next = NULL;
	head = pwd;
	add_4_element(head);
	return (head);
}
