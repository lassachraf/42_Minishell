/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_expand.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 12:54:58 by alassiqu          #+#    #+#             */
/*   Updated: 2024/06/13 23:21:29 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*expand_wildcard(void)
{
	DIR				*dir;
	struct dirent	*entry;
	char			*expanded;
	size_t			len;
	size_t			allocated;
	size_t			name_len;
	char			*new_expanded;

	len = 0;
	allocated = 256;
	dir = opendir(".");
	if (!dir)
		return (perror("opendir failed!"), NULL);
	expanded = malloc(allocated);
	if (!expanded)
		return (perror("malloc failed!"), closedir(dir), NULL);
	expanded[0] = '\0';
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (entry->d_name[0] != '.')
		{
			name_len = ft_strlen(entry->d_name);
			if (len + name_len + 2 > allocated)
			{
				allocated *= 2;
				new_expanded = realloc(expanded, allocated);
				if (!new_expanded)
					return (perror("realloc"), free(expanded), closedir(dir),
						NULL);
				expanded = new_expanded;
			}
			if (len > 0)
			{
				strcat(expanded, " ");
				len++;
			}
			strcat(expanded, entry->d_name);
			len += name_len;
		}
		entry = readdir(dir);
	}
	closedir(dir);
	return (expanded);
}

bool	match(char *pattern, char *candidate, int p, int c)
{
	if (pattern[p] == '\0')
		return (candidate[c] == '\0');
	else if (pattern[p] == '*')
	{
		while (candidate[c] != '\0')
		{
			if (match(pattern, candidate, p + 1, c))
				return (true);
			c++;
		}
		return (match(pattern, candidate, p + 1, c));
	}
	else if (pattern[p] != '?' && pattern[p] != candidate[c])
		return (false);
	else
		return (match(pattern, candidate, p + 1, c + 1));
}

char	*expand_wildcard(char *wd)
{
	DIR *dir;
	struct dirent *entry;
	char **all_dirs;
	int i;

	dir = opendir(".");
	if (!dir)
		return (error("opendir() failed", NULL), NULL);
	all_dirs = NULL;
	i = 0;
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] != '.' && match(wd, entry->d_name, 0, 0))
			add_filename(&all_dirs, &i, ft_strdup(entry->d_name));
		entry = readdir(dir);
	}
	bubble_sort(all_dirs, i);
	closedir(dir);
	return (all_dirs);
}