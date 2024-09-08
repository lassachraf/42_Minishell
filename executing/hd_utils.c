/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 18:12:59 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/07 19:15:29 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	set_filename(char *filename, unsigned char *buff, int length)
{
	int	val;
	int	i;

	i = -1;
	val = 0;
	while (++i < length)
	{
		val = buff[i] % 62;
		if (val < 26)
			filename[i] = 'a' + val;
		else if (val < 52)
			filename[i] = 'A' + (val - 26);
		else
			filename[i] = '0' + (val - 52);
	}
	filename[length] = '\0';
}

int	generate_random_filename(char *filename, int length)
{
	unsigned char	*buffer;
	int				fd;

	buffer = malloc(sizeof(unsigned char) * (length));
	if (!buffer)
		return (perror("Allocation failed !"), -1);
	gc_add(g_minishell, buffer);
	fd = open("/dev/random", O_RDONLY);
	if (fd < 0)
	{
		perror("Failed to open /dev/random");
		return (-1);
	}
	if (read(fd, buffer, length) != length)
	{
		perror("Failed to read from /dev/random");
		close(fd);
		return (-1);
	}
	close(fd);
	set_filename(filename, buffer, length - 1);
	return (0);
}

char	*build_file_name(int id)
{
	t_list		*file;
	char		*name;
	char		nb[20];

	if (generate_random_filename(nb, 20))
		return (NULL);
	name = ft_strjoin("badashell_file_", nb);
	gc_add(g_minishell, name);
	file = ft_lstnew(name);
	file->id = id;
	gc_add(g_minishell, file);
	ft_lstadd_back(&g_minishell->files, file);
	return (name);
}
