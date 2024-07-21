/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_expand.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 12:54:58 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/19 10:09:43 by alassiqu         ###   ########.fr       */
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

int	check_ambiguous(t_token *curr)
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
			&& match_pattern(curr->value, entry->d_name))
		{
			match_count++;
		}
		entry = readdir(dir);
	}
	closedir(dir);
	return (match_count);
} 

t_list	*asterisk_functionality(char *s)
{
	DIR				*dir;
	struct dirent	*entry;
	t_list			*lst;
	int				i;

	lst = NULL;
	dir = opendir(".");
	if (!dir)
		return (NULL);
	i = 0;
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] != '.' && match_pattern(s,
				entry->d_name))
		{			
			ft_lstadd_back(&lst, ft_lstnew(entry->d_name));
			i++;
		}
		entry = readdir(dir);
	}
	closedir(dir);
	return (lst);
}

void	adjust_token(t_token **curr, char *new)
{
	char	*str;

	str = ft_strdup(new);
	(*curr)->value = ft_strjoin(str, (*curr)->value);
	gc_add(g_minishell, (*curr)->value);
	free(str);
	(*curr) = (*curr)->next;
}

// void	asterisk_expand(t_token **tokens, t_token *curr)
// {
// 	int		i;

// 	print_tokens(g_minishell->tokens);
// 	if (curr->prev && curr->prev->type >= RR_REDIR
// 		&& curr->prev->type <= R_REDIR)
// 	{
// 		i = check_ambiguous(curr);
// 		if (i > 1)
// 		{
// 			adjust_token(&curr, "*2");
// 			return ;
// 		}
// 		else if (i == 0)
// 		{
// 			adjust_token(&curr, "*0");
// 			return ;
// 		}
// 		else
// 			i = asterisk_functionality(tokens, curr);
// 	}
// 	else
// 		i = asterisk_functionality(tokens, curr);
// 	if (i)
// 		remove_token(tokens, curr);
// 	print_tokens(g_minishell->tokens);
// }
