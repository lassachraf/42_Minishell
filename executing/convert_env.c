/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:17:38 by baouragh          #+#    #+#             */
/*   Updated: 2024/08/31 17:13:33 by alassiqu         ###   ########.fr       */
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

void	select_and_excute(t_node *node, int type, int *pfd)
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

int	last_element(char **s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i - 1);
}

t_list	*expand_a_dollar(char **s, bool avoid, char *value, int n)
{
	char	**split;

	printf(">> %d\n", n);
	if (avoid)
		*s = avoid_spaces(helper_expander(*s));
	else
		*s = helper_expander(*s);
	if (n == 2) // export a=$b
		return (NULL);
	else
	{
		split = ft_split(*s, ' ');
		if (!split)
			return (NULL);
		int index = last_element(split);
		if (index == -1)
			return (NULL);
		gc_add(g_minishell, split[index]);
		split[index] = ft_strjoin(split[index], value);
		printf("split[index] >> %s\n", split[index]);
		return (creat_list(split, 1));
	}
}

int	setting_data(t_list **s, char **tmp, char **key, char **value)
{
	int	len;
	int	start;

	*tmp = ft_strchr((char *)(*s)->content, '=');
	len = (ft_strlen((char *)(*s)->content) - (*tmp - (char *)(*s)->content));
	start = (*tmp - (char *)(*s)->content);
	*key = ft_substr((char *)(*s)->content, 0, start);
	gc_add(g_minishell, *key);
	*value = ft_substr((char *)(*s)->content, start, len);
	gc_add(g_minishell, *value);
	return (check_dollars((char *)(*s)->content));
}

void	split_case_3(t_list **s, char **key, char *value, int split)
{
	t_list	*tmp;
	char	**val;
	char	*temp;
	int		p;

	val = ft_split(helper_expander(value), ' ');
	add_list_into_list(s, expand_a_dollar(key, 0, val[0], split));
	p = 0;
	while (val[++p])
	{
		temp = ft_strdup(val[p]);
		gc_add(g_minishell, temp);
		tmp = ft_lstnew(temp);
		gc_add(g_minishell, tmp);
		ft_lstadd_back(s, tmp);
	}
	free_double(val);
}

int	check_for_export(t_list **s, bool *avoid)
{
	char	*tmp;
	char	*key;
	t_list	*list;
	char	*value;
	int		split;

	tmp = NULL;
	tmp = NULL;
	key = NULL;
	value = NULL;
	(*s) = (*s)->next;
	list = *s;
	while(s && *s && (*s)->content)
	{
		split = setting_data(s, &tmp, &key, &value);
		if (split == 1)
			add_list_into_list(s, expand_a_dollar(&key, 0, value, split));
		else if (split == 2)
			(*s)->content = helper_expander((*s)->content);
		else if (split == 3)
			split_case_3(s, &key, value, split);
		(*s) = (*s)->next;
	}
	*s = list;
	return ((*avoid) = 0, 0);
}
