/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:17:38 by baouragh          #+#    #+#             */
/*   Updated: 2024/08/31 17:57:26 by alassiqu         ###   ########.fr       */
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

void	select_and_execute(t_node *node, int type, int *pfd)
{
	g_minishell->last_child = fork();
	if (!g_minishell->last_child)
	{
		if (type == STRING_NODE)
			execute_cmd(node, pfd);
		else if (type == PAIR_NODE)
			execute_pair(node, pfd);
		else
			execute_redires(node->data.redir, pfd);
		wait_last();
		wait_all();
		exit(g_minishell->exit_s);
	}
	else
		wait_last();
}

int	asterisk_functionality_2(t_list **cmds, char *s)
{
	t_list			*lst;
	DIR				*dir;
	struct dirent	*entry;

	lst = NULL;
	dir = opendir(".");
	if (!dir)
		return (0);
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] != '.' && match_pattern(s,
				entry->d_name))
			add_name_to_list(&lst, entry->d_name);
		else if (entry->d_name[0] == '.' && s[0] == '.'
			&& match_pattern(s, entry->d_name))
			add_name_to_list(&lst, entry->d_name);
		entry = readdir(dir);
	}
	closedir(dir);
	add_list_into_list(cmds, lst);
	return (1);
}