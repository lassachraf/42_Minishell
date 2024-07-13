/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_expand.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 12:54:58 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/13 14:12:23 by alassiqu         ###   ########.fr       */
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

// // ambiguous redirect

int	check_ambiguous(t_token *curr)
{
	DIR				*dir;
	struct dirent	*entry;
	int				match_count;

	match_count = 0;
	dir = opendir(".");
	if (!dir)
		return (100);
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] != '.' && match_pattern(curr->value, entry->d_name))
		{
			match_count++;
		}
		entry = readdir(dir);
	}
	closedir(dir);
	return (match_count);
}

int	asterisk_functionality(t_token **tokens, t_token *curr)
{
	DIR				*dir;
	struct dirent	*entry;
	int				i;

	dir = opendir(".");
	if (!dir)
		return (-1);
	i = 0;
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
	return (i);
}

void	asterisk_expand(t_token **tokens, t_token *curr)
{
	int	i;

	if (curr->prev && curr->prev->type >= RR_REDIR && curr->prev->type <= R_REDIR)
	{
		i = check_ambiguous(curr);
		if (i > 1)
		{
			curr->value = ft_strdup("*2");
			gc_add(g_minishell, curr->value);
			curr = curr->next;
			return ;
		}
		else if (i == 0)
		{
			curr->value = ft_strdup("*0");
			gc_add(g_minishell, curr->value);
			curr = curr->next;
			return ;
		}
		else
			i = asterisk_functionality(tokens, curr);
	}
	else
		i = asterisk_functionality(tokens, curr);
	if (i)
		remove_token(tokens, curr);
}
