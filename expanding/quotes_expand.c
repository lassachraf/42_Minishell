/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_expand.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 09:13:39 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/02 11:18:33 by alassiqu         ###   ########.fr       */
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

void	fill(char *new_result, char *str, int len, int newline)
{
	int	str_len;

	str_len = ft_strlen(str);
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
}

void	append_to_result(char **result, const char *str, int newline)
{
	char	*new_result;
	size_t	str_len;
	size_t	len;

	len = 0;
	str_len = ft_strlen(str);
	if (*result)
		len = ft_strlen(*result);
	if (newline)
		new_result = malloc(len + str_len + 2);
	else
		new_result = malloc(len + str_len + 3);
	if (!new_result)
		return ;
	if (*result)
	{
		ft_strcpy(new_result, *result);
		free(*result);
	}
	fill(new_result, str, len, newline);
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

// int	custome_expand(t_token *token)
// {
// 	if (token->prev == WORD)
// 	{
		
// 	}
// }

void	asterisk_expand(t_token *token)
{
	DIR		*dir;
	char	*result;

	result = NULL;
	dir = opendir(".");
	if (!dir)
		return ;
	process_files(dir, &result, token->value);
	dir = opendir(".");
	if (!dir)
		return ;
	process_dirs(dir, &result, token->value);
	if (result)
	{
		free(token->value);
		token->value = result;
		token->type = WORD;
	}
	token = token->next;
}
