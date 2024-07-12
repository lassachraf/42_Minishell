/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_expand.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 12:54:58 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/12 18:03:28 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_strcpy(char *dest, const char *src)
{
	int	i;

	i = -1;
	while (src[++i])
		dest[i] = src[i];
	dest[i] = '\0';
	return (dest);
}

void	append_to_result(char **result, char *str, int newline)
{
	size_t	len;
	size_t	str_len;
	char	*new_result;

	len = 0;
	str_len = ft_strlen(str);
	if (*result)
		len = ft_strlen(*result);
	new_result = malloc(len + str_len + (newline ? 2 : 3));
	if (!new_result)
		return ;
	if (*result)
	{
		ft_strcpy(new_result, *result);
		free(*result);
	}
	ft_strcpy(new_result + len, str);
	if (newline == 1)
	{
		new_result[len + str_len] = '\n';
		new_result[len + str_len + 1] = '\0';
	}
	else if (newline == 2)
	{
		new_result[len + str_len] = ':';
		new_result[len + str_len + 1] = '\0';
	}
	else
	{
		new_result[len + str_len] = ' ';
		new_result[len + str_len + 1] = ' ';
		new_result[len + str_len + 2] = '\0';
	}
	*result = new_result;
}

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

void	sort_strings(char **strings, size_t count)
{
	char	*temp;
	size_t	i;
	size_t	j;

	i = 0;
	while (i < count - 1)
	{
		j = i + 1;
		while (j < count)
		{
			if (ft_strcmp(strings[i], strings[j]) > 0)
			{
				temp = strings[i];
				strings[i] = strings[j];
				strings[j] = temp;
			}
			j++;
		}
		i++;
	}
}

void asterisk_expand(t_token **tokens, t_token *curr)
{
    DIR				*dir;
    struct dirent	*entry;

	dir = opendir(".");
    if (!dir)
	{
        return ;
	}
	entry = readdir(dir);
    while (entry)
	{
        if (entry->d_name[0] != '.' && match_pattern(curr->value, entry->d_name))
            add_token_middle(tokens, new_token(ft_strdup(entry->d_name), WORD), curr->prev);
		entry = readdir(dir);
    }
    closedir(dir);
	remove_token(tokens, curr);
}
