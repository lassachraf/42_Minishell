/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:10:33 by baouragh          #+#    #+#             */
/*   Updated: 2024/07/17 09:00:42 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	print_err(char *message, char *word)
{
	ft_putstr_fd(RED, 2);
	ft_putstr_fd("badashell$ : ", 2);
	ft_putstr_fd(word, 2);
	ft_putstr_fd(" : ", 2);
	ft_putstr_fd(message, 2);
	ft_putstr_fd("\n" RESET, 2);
	return (0);
}

void	check_split(char **cmd, char *word)
{
	if (!cmd)
	{
		print_err("malloc failed in ft_split !!", word);
		if (!word)
			ft_putstr_fd("NULL\n", 2);
		return ;
	}
}

int	strings_count(char **str)
{
	char	**save;

	if (!str)
		return (-1);
	save = str;
	while (*str)
		str++;
	return (str - save);
}

void	free_double(char **ptr)
{
	int	i;

	i = 0;
	if (!ptr || !*ptr)
		return ;
	while (ptr[i])
	{
		free(ptr[i]);
		i++;
	}
	free(ptr);
}

char	*founded_cmd(char *argv, char **paths, char **cmd)
{
	char	*fullpath;

	fullpath = get_command(argv);
	if (!fullpath)
	{
		free_double(paths);
		free_double(cmd);
		return (NULL);
	}
	return (free_double(paths), free_double(cmd), fullpath);
}
