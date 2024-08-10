/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:17:38 by baouragh          #+#    #+#             */
/*   Updated: 2024/08/08 01:07:31 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	env_size(t_env *env)
{
	int	size;

	size = 0;
	while (env)
	{
		if (env->visible)
			size++;
		env = env->next;
	}
	return (size);
}

char	**env_to_envp(t_env *env)
{
	char	**argv;
	char	*tmp;
	int		size;
	int		i;

	i = 0;
	size = env_size(env);
	argv = malloc(sizeof(char *) * (size + 1));
	if (!argv)
		return (NULL);
	gc_add(g_minishell, argv);
	while (i < size)
	{
		tmp = ft_strjoin(env->key, "=");
		argv[i] = ft_strjoin(tmp, env->value);
		if (!argv[i])
			return (gc_free_all(g_minishell), NULL);
		gc_add(g_minishell, argv[i]);
		free(tmp);
		env = env->next;
		i++;
	}
	argv[i] = NULL;
	return (argv);
}

char	**list_to_argv(t_list *list)
{
	char	**argv;
	int		size;
	int		i;
	int		len;

	i = 0;
	if (!list || !list->content)
		return (NULL);
	size = ft_lstsize(list);
	argv = malloc(sizeof(char *) * (size + 1));
	if (!argv)
		return (gc_free_all(g_minishell), NULL);
	gc_add(g_minishell, argv);
	while (i < size)
	{
		len = ft_strlen(list->content) + 1;
		argv[i] = malloc(sizeof(char) * len);
		if (!argv[i])
			return (gc_free_all(g_minishell), NULL);
		gc_add(g_minishell, argv[i]);
		ft_memmove(argv[i++], list->content, len);
		list = list->next;
	}
	argv[i] = NULL;
	return (argv);
}

void	select_and_excute(t_node *node, int type)
{
	g_minishell->last_child = fork();
	if (!g_minishell->last_child)
	{
		if (type == STRING_NODE)
			execute_cmd(node);
		else if (type == PAIR_NODE)
			execute_pair(node);
		else
			execute_redires(node->data.redir);
		wait_last();
		wait_all();
		exit(g_minishell->exit_s);
	}
	else
		wait_last();
}

void	check_for_export(void *s, bool *avoid)
{
	if (s && !ft_strcmp((char *)s, "export"))
		(*avoid) = 0;
}
