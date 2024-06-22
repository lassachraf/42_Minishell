/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_expand.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 12:54:58 by alassiqu          #+#    #+#             */
/*   Updated: 2024/06/22 22:22:00 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// char	*ft_strcpy(char *dest, char *src)
// {
// 	int	i;

// 	i = 0;
// 	while (src[i] != '\0')
// 	{
// 		dest[i] = src[i];
// 		i++;
// 	}
// 	dest[i] = '\0';
// 	return (dest);
// }

// void	append_to_result(char **result, char *str)
// {
// 	size_t	len;
// 	size_t	str_len;
// 	char	*new_result;

// 	len = 0;
// 	if (*result)
// 		len = ft_strlen(*result);
// 	str_len = ft_strlen(str);
// 	new_result = malloc(len + str_len + 2);
// 	if (!new_result)
// 		return ;
// 	if (*result)
// 	{
// 		ft_strcpy(new_result, *result);
// 		free(*result);
// 	}
// 	ft_strcpy(new_result + len, str);
// 	new_result[len + str_len] = '\n';
// 	new_result[len + str_len + 1] = '\0';
// 	*result = new_result;
// }

// int	match_pattern(const char *pattern, const char *filename)
// {
// 	while (*pattern && *filename)
// 	{
// 		if (*pattern == '*')
// 		{
// 			if (*(pattern + 1) == '\0')
// 				return (1);
// 			while (*filename)
// 			{
// 				if (match_pattern(pattern + 1, filename++))
// 					return (1);
// 			}
// 			return (0);
// 		}
// 		else if (*pattern != *filename)
// 		{
// 			return (0);
// 		}
// 		pattern++;
// 		filename++;
// 	}
// 	return ((*pattern == '*' && *(pattern + 1) == '\0')
// 		|| (*pattern == *filename));
// }

// void	asterisk_expand(t_token *token)
// {
// 	DIR				*dir;
// 	struct dirent	*entry;
// 	char			*dir_path;
// 	char			*result;

// 	dir_path = ".";
// 	result = NULL;
// 	dir = opendir(dir_path);
// 	if (!dir)
// 		return ;
// 	entry = readdir(dir);
// 	while (entry)
// 	{
// 		if (match_pattern(token->value, entry->d_name))
// 			append_to_result(&result, entry->d_name);
// 		entry = readdir(dir);
// 	}
// 	closedir(dir);
// 	if (result)
// 	{
// 		free(token->value);
// 		token->value = result;
// 		token->type = WORD;
// 	}
// }

////////////////////////////////////////////////////////////////////////////

char *ft_strcpy(char *dest, const char *src)
{
	int	i;

	i = 0;
	while (src[i] != '\0') {
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

void append_to_result(char **result, const char *str, int newline) {
    size_t len = 0;
    size_t str_len = ft_strlen(str);
    char *new_result;

    if (*result)
        len = ft_strlen(*result);
    new_result = malloc(len + str_len + (newline ? 2 : 3)); // Extra space for "\n" or "  "
    if (!new_result)
        return;
    if (*result) {
        ft_strcpy(new_result, *result);
        free(*result);
    }
    ft_strcpy(new_result + len, str);
    if (newline)
	{
        new_result[len + str_len] = '\n';
        new_result[len + str_len + 1] = '\0';
    }
	else {
        new_result[len + str_len] = ' ';
        new_result[len + str_len + 1] = ' ';
        new_result[len + str_len + 2] = '\0';
    }
    *result = new_result;
}

int match_pattern(const char *pattern, const char *filename) {
    while (*pattern && *filename) {
        if (*pattern == '*') {
            if (*(pattern + 1) == '\0')
                return 1;
            while (*filename) {
                if (match_pattern(pattern + 1, filename++))
                    return 1;
            }
            return 0;
        } else if (*pattern != *filename) {
            return 0;
        }
        pattern++;
        filename++;
    }
    return ((*pattern == '*' && *(pattern + 1) == '\0') || (*pattern == *filename));
}

void process_directory(char *dir_name, char **result) {
    DIR *dir;
    struct dirent *entry;
    char *dir_header;
    size_t len;

    dir = opendir(dir_name);
    len = ft_strlen(dir_name);
    if (!dir)
        return;
    dir_header = malloc(len + 2);
    ft_strcpy(dir_header, dir_name);
    dir_header[len] = ':';
    dir_header[len + 1] = '\0';
    append_to_result(result, dir_header, 1);
    free(dir_header);

    entry = readdir(dir);
    while (entry) {
        if (entry->d_name[0] != '.') {
            char *indented_entry = malloc(ft_strlen(entry->d_name) + 3);
            sprintf(indented_entry, "  %s", entry->d_name);
            append_to_result(result, indented_entry, 1);
            free(indented_entry);
        }
        entry = readdir(dir);
    }
    append_to_result(result, "", 1);
    closedir(dir);
}

void process_files(DIR *dir, char **result, const char *pattern) {
    struct dirent *entry;
    int first_file = 1;

    entry = readdir(dir);
    while (entry) {
        if (entry->d_name[0] != '.' && match_pattern(pattern, entry->d_name)) {
            DIR *subdir = opendir(entry->d_name);
            if (!subdir) { // It's a file, not a directory
                if (!first_file) {
                    append_to_result(result, entry->d_name, 0); // No newline
                } else {
                    append_to_result(result, entry->d_name, 0); // First file
                    first_file = 0;
                }
            } else {
                closedir(subdir);
            }
        }
        entry = readdir(dir);
    }
    append_to_result(result, "", 1); // Add a newline after files
}

void asterisk_expand(t_token *token) {
    DIR *dir;
    struct dirent *entry;
    char *dir_path;
    char *result = NULL;

    dir_path = ".";
    dir = opendir(dir_path);
    if (!dir)
        return;
    process_files(dir, &result, token->value);
    closedir(dir);
    dir = opendir(dir_path);
    if (!dir)
        return;
    entry = readdir(dir);
    while (entry) {
        if (entry->d_name[0] != '.' && match_pattern(token->value, entry->d_name)) {
            DIR *subdir = opendir(entry->d_name);
            if (subdir) {
                closedir(subdir);
                process_directory(entry->d_name, &result);
            }
        }
        entry = readdir(dir);
    }
    closedir(dir);

    if (result) {
        free(token->value);
        token->value = result;
        token->type = WORD;
    }
}
