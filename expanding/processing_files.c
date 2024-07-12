/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processing_files.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 09:37:59 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/12 17:03:05 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

size_t	count_files(DIR *dir, const char *pattern)
{
	DIR				*subdir;
	struct dirent	*entry;
	size_t			count;

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

void	get_files(DIR *dir, struct dirent *entry, char **file_list,
		const char *pattern)
{
	size_t	i;

	i = 0;
	(void)dir;
	while (entry)
	{
		if (entry->d_name[0] != '.' && match_pattern(pattern, entry->d_name))
		{
			file_list[i] = strdup(entry->d_name);
			i++;
		}
		entry = readdir(dir);	
	}
	if (!file_list)
	{
		printf("NULLLLLLLLLLLLL\n");
		return ;
	}
	i = 0;
	while (file_list[i])
	{
		printf("** `%zu` >> `%s` **\n", i, file_list[i]);
		i++;
	}
}

void	append_files(t_token **tokens, t_token *prev, char **file_list, int file_count)
{
	int	i;

	i = 0;
	while (i < file_count)
	{
		add_token_middle(tokens, new_token(file_list[i], WORD), prev);
		free(file_list[i]);
		i++;
	}
	free(file_list);
}

void	process_files(t_token **tokens, t_token *prev, char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**file_list = NULL;
	int				file_count;
	int				i;

	dir = opendir(".");
	if (!dir)
		return ;
	file_count = count_files(dir, pattern);
	entry = readdir(dir);
	i = 0;
	get_files(dir, entry, file_list, pattern);
	sort_strings(file_list, file_count);
	append_files(tokens, prev, file_list, file_count);
	closedir(dir);
	print_tokens(*tokens);
}
