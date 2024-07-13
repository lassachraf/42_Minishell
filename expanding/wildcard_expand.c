/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_expand.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 12:54:58 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/12 22:43:31 by alassiqu         ###   ########.fr       */
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

void	asterisk_expand(t_token **tokens, t_token *curr)
{
	DIR				*dir;
	struct dirent	*entry;
	int				i;

	i = 0;
	dir = opendir(".");
	if (!dir)
	{
		return ;
	}
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] != '.' && match_pattern(curr->value,
				entry->d_name))
		{
			add_token_middle(tokens, new_token(ft_strdup(entry->d_name), WORD),
				curr->prev);
			i++;
		}
		entry = readdir(dir);
	}
	closedir(dir);
	if (i)
		remove_token(tokens, curr);
}
