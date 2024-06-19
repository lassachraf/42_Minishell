/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_expand.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 12:54:58 by alassiqu          #+#    #+#             */
/*   Updated: 2024/06/18 16:08:28 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// char	*expand_wildcard(void)
// {
// 	DIR				*dir;
// 	struct dirent	*entry;
// 	char			*expanded;
// 	size_t			len;
// 	size_t			allocated;
// 	size_t			name_len;
// 	char			*new_expanded;
// 	len = 0;
// 	allocated = 256;
// 	dir = opendir(".");
// 	if (!dir)
// 		return (perror("opendir failed!"), NULL);
// 	expanded = malloc(allocated);
// 	if (!expanded)
// 		return (perror("malloc failed!"), closedir(dir), NULL);
// 	expanded[0] = '\0';
// 	entry = readdir(dir);
// 	while (entry != NULL)
// 	{
// 		if (entry->d_name[0] != '.')
// 		{
// 			name_len = ft_strlen(entry->d_name);
// 			if (len + name_len + 2 > allocated)
// 			{
// 				allocated *= 2;
// 				new_expanded = realloc(expanded, allocated);
// 				if (!new_expanded)
// 					return (perror("realloc"), free(expanded), closedir(dir),
// 						NULL);
// 				expanded = new_expanded;
// 			}
// 			if (len > 0)
// 			{
// 				strcat(expanded, " ");
// 				len++;
// 			}
// 			strcat(expanded, entry->d_name);
// 			len += name_len;
// 		}
// 		entry = readdir(dir);
// 	}
// 	closedir(dir);
// 	return (expanded);
// }

char	*ft_strcpy(char *dest, char *src)
{
	int	i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

void	append_to_result(char **result, char *str)
{
	size_t	len;
	size_t	str_len;
	char	*new_result;

	len = 0;
	if (*result)
		len = ft_strlen(*result);
	str_len = ft_strlen(str);
	new_result = malloc(len + str_len + 2);
	if (!new_result)
		return ;
	if (*result)
	{
		ft_strcpy(new_result, *result);
		free(*result);
	}
	ft_strcpy(new_result + len, str);
	new_result[len + str_len] = '\n';
	new_result[len + str_len + 1] = '\0';
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

void	asterisk_expand(t_token *token)
{
	DIR				*dir;
	struct dirent	*entry;
	char			*dir_path;
	char			*result;

	dir_path = ".";
	result = NULL;
	dir = opendir(dir_path);
	if (!dir)
		return ;
	entry = readdir(dir);
	while (entry)
	{
		if (match_pattern(token->value, entry->d_name))
			append_to_result(&result, entry->d_name);
		entry = readdir(dir);
	}
	closedir(dir);
	if (result)
	{
		free(token->value);
		token->value = result;
		// token->type = WORD; should be asterisk as a default.
	}
}
