/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processing_files.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 09:37:59 by alassiqu          #+#    #+#             */
/*   Updated: 2024/06/23 10:04:09 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

size_t	count_files(DIR *dir, const char *pattern)
{
	DIR				*subdir;
	struct dirent	*entry;
	size_t 			count;	

	count = 0;
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] != '.' && match_pattern(pattern, entry->d_name))
		{
			subdir = opendir(entry->d_name);
			if (!subdir)
				count++;
			else
				closedir(subdir);
		}
		entry = readdir(dir);
	}
	closedir(dir);
	return (count);
}

void	get_files(DIR *dir, struct dirent *entry, char **file_list, const char *pattern)
{
	DIR		*subdir;
	size_t	i;

	i = 0;
	while (entry)
	{
		if (entry->d_name[0] != '.' && match_pattern(pattern, entry->d_name))
		{
			subdir = opendir(entry->d_name);
			if (!subdir)
			{
				file_list[i] = strdup(entry->d_name);
				i++;
			} 
			else
				closedir(subdir);
		}
		entry = readdir(dir);
	}
}

void	append_files(char **result, char **file_list ,int file_count)
{
	int	i;

	i = 0;
	while (i < file_count)
	{
        append_to_result(result, file_list[i], 0);
        free(file_list[i]);
		i++;
    }
    append_to_result(result, "", 1);
    free(file_list);
}

void	process_files(DIR *dir, char **result, const char *pattern)
{
	struct dirent	*entry;
	char			**file_list;
	size_t			file_count;

	file_count = count_files(dir, pattern);
	dir = opendir(".");
	if (!dir)
		return ;
	file_list = malloc(sizeof(char *) * file_count);
	if (!file_list)
		return ;
	entry = readdir(dir);
	get_files(dir, entry, file_list, pattern);
	sort_strings(file_list, file_count);
	append_files(result, file_list, file_count);
	closedir(dir);
}
