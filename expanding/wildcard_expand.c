/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_expand.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 12:54:58 by alassiqu          #+#    #+#             */
/*   Updated: 2024/09/01 11:43:10 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	match_pattern(const char *pattern, const char *filename)
{
	while (*pattern && *filename)
	{
		if (*pattern == '*')
		{
			if (*(pattern + 1) == '\0')
				return (1);
			while (*filename)
			{
				if (match_pattern(pattern + 1, filename++))
					return (1);
			}
			return (0);
		}
		else if (*pattern != *filename)
		{
			return (0);
		}
		pattern++;
		filename++;
	}
	return ((*pattern == '*' && *(pattern + 1) == '\0')
		|| (*pattern == *filename));
}

int	check_ambiguous(char *s)
{
	DIR				*dir;
	struct dirent	*entry;
	int				match_count;

	match_count = 0;
	dir = opendir(".");
	if (!dir)
		return (-1);
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] != '.'
			&& match_pattern(s, entry->d_name))
		{
			match_count++;
		}
		entry = readdir(dir);
	}
	closedir(dir);
	return (match_count);
}

void	insert_sorted(t_list **sorted, t_list *new_node)
{
	t_list	*current;

	if (*sorted == NULL
		|| ft_strcasecmp((*sorted)->content, new_node->content) > 0)
	{
		new_node->next = *sorted;
		*sorted = new_node;
	}
	else
	{
		current = *sorted;
		while (current->next
			&& ft_strcasecmp(current->next->content, new_node->content) <= 0)
			current = current->next;
		new_node->next = current->next;
		current->next = new_node;
	}
}

void	add_name_to_list(t_list **lst, char *dir_name)
{
	t_list	*new_node;
	char	*name;	

	name = ft_strdup(dir_name);
	gc_add(g_minishell, name);
	new_node = ft_lstnew(name);
	gc_add(g_minishell, new_node);
	insert_sorted(lst, new_node);
}

t_list	*asterisk_functionality(char *s)
{
	DIR				*dir;
	t_list			*lst;
	struct dirent	*entry;

	lst = NULL;
	dir = opendir(".");
	if (!dir)
		return (NULL);
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
	return (lst);
}
