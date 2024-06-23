/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processing_dirs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 09:37:57 by alassiqu          #+#    #+#             */
/*   Updated: 2024/06/23 11:24:08 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

size_t	count_dirs(DIR *dir)
{
	struct dirent	*entry;
	size_t			count;

	count = 0;
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] != '.' && entry->d_type == DT_DIR)
			count++;
		entry = readdir(dir);
	}
	closedir(dir);
	return (count);
}

void	get_dirs(DIR *dir, struct dirent *entry, char **dir_list)
{
	size_t	i;

	i = 0;
	while (entry)
	{
		if (entry->d_name[0] != '.' && entry->d_type == DT_DIR)
		{
			dir_list[i] = strdup(entry->d_name);
			i++;
		}
		entry = readdir(dir);
	}
}

void	append_dirs(char **result, char **dir_list, const char *pattern,
		int dir_count)
{
	char	*old_dir;
	int		i;

	i = 0;
	while (i < dir_count)
	{
		append_to_result(result, dir_list[i], 2);
		append_to_result(result, "", 1);
		old_dir = strdup(dir_list[i]);
		chdir(dir_list[i]);
		process_files(opendir("."), result, pattern);
		chdir("..");
		append_to_result(result, "", 1);
		free(old_dir);
		free(dir_list[i]);
		i++;
	}
	free(dir_list);
}

void	process_dirs(DIR *dir, char **result, const char *pattern)
{
	struct dirent	*entry;
	char			**dir_list;
	size_t			dir_count;

	dir_count = count_dirs(dir);
	dir = opendir(".");
	if (!dir)
		return ;
	dir_list = malloc(sizeof(char *) * dir_count);
	if (!dir_list)
		return ;
	append_to_result(result, "", 1);
	entry = readdir(dir);
	get_dirs(dir, entry, dir_list);
	sort_strings(dir_list, dir_count);
	append_dirs(result, dir_list, pattern, dir_count);
	closedir(dir);
}
