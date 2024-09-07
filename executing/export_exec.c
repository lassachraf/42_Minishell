/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 17:20:19 by alassiqu          #+#    #+#             */
/*   Updated: 2024/09/06 17:28:49 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	last_element(char **s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i - 1);
}

t_list	*expand_a_dollar(char **s, int *n, char *value, int quote)
{
	char	**split;
	int		index;

	if (n[0] && quote)
		*s = avoid_spaces(helper_expander(*s));
	else
		*s = helper_expander(*s);
	if (n[1] == 2)
		return (NULL);
	else
	{
		split = ft_split(*s, ' ');
		if (!split)
			return (NULL);
		index = last_element(split);
		if (index == -1)
			return (NULL);
		gc_add(g_minishell, split[index]);
		split[index] = ft_strjoin(split[index], value);
		return (creat_list(split, 1));
	}
	return (NULL);
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
	int		*p2;
	int		p;

	p2 = malloc(sizeof(int) * 2);
	if (p2)
		return ;
	gc_add(g_minishell, p2);
	p2[0] = 0;
	p2[1] = split;
	val = ft_split(helper_expander(value), ' ');
	add_list_into_list(s, expand_a_dollar(key, p2, val[0], (*s)->is_quoted));
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

int	check_for_export(t_list **s, bool *avoid, int *flag)
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
	while (s && *s && (*s)->content)
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
	return ((*avoid) = 0, (*flag) = 1, 0);
}
