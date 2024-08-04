/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:11:26 by baouragh          #+#    #+#             */
/*   Updated: 2024/08/04 02:26:35 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**get_env_paths(void)
{
	char	**res;
	char	*tmp;

	tmp = get_env_var(g_minishell->our_env, "PATH");
	if (!tmp)
		return (NULL);
	res = ft_split(tmp, ':');
	if (!res)
		return (NULL);
	return (res);
}

bool	get_path_data(char *argv, char	***paths, char	***cmd, int *paths_num)
{
	(*paths) = get_env_paths();
	if (!(*paths))
		return (0);
	*paths_num = strings_count((*paths));
	if (*paths_num == -1 || !*paths_num)
		return (free_double((*paths)), 0);
	*cmd = ft_split(argv, ' ');
	if (!*cmd)
		return (free_double((*paths)), 0);
	return (1);
}

char	*get_fullpath(char *argv)
{
	int		paths_num;
	char	**cmd;
	char	**paths;
	char	*fullpath;
	int		i;

	if (!argv || !*argv || ft_isspace(*argv))
		return (NULL);
	fullpath = NULL;
	i = 0;
	if (!get_path_data(argv, &paths, &cmd, &paths_num))
		return (NULL);
	if (!(access(*cmd, F_OK)))
	{
		if ((*argv == '/' || *argv == '.') && !access(*cmd, X_OK))
			return (founded_cmd(argv, paths, cmd));
	}
	else if (*argv != '/' && *argv != '.')
		while (paths_num-- > 0 && !fullpath)
			fullpath = add_slash_cmd(paths[i++], *cmd);
	return (free_double(cmd), free_double(paths), fullpath);
}

int	check_cmd(char *argv)
{
	char		*cmd;
	struct stat	statbuf;

	statbuf.st_mode = 0;
	stat(argv, &statbuf);
	if (S_ISDIR(statbuf.st_mode) == true && ft_strchr(argv, '/'))
		return (print_err("Is a directory", argv), 126);
	cmd = get_fullpath(argv);
	gc_add(g_minishell, cmd);
	if (!cmd && *argv == '.')
		cmd = get_command(argv);
	if (*argv != '\0' && (*argv == '/' || *argv == '.'
			|| !get_env_var(g_minishell->our_env, "PATH")) && access(cmd, F_OK))
		return (print_err("No such file or directory", argv), 127);
	else if (ft_strlen(argv) == 1 && argv[0] == '.')
	{
		print_err("filename argument required\n.: usage: . filename [arguments]",
			NULL);
		return (2);
	}
	else if (access(cmd, F_OK) || (argv[0] == '.' && argv[1] == '.'))
		return (print_err("Command not found", argv), 127);
	else if (access(cmd, X_OK))
		return (print_err("Permission denied", argv), 126);
	return (0);
}

void	call_execev(char **env, char *argv, char **cmd)
{
	char	*founded_path;
	int		len;

	len = ft_strlen(argv);
	founded_path = get_fullpath(argv);
	printf(RED);
	execve(founded_path, cmd, env);
	printf(RESET);
	print_err("EXEVE FAILED ", NULL);
}

int	ft_malloc_error(char **tab, size_t i)
{
	size_t	j;

	j = 0;
	while (j <= i)
	{
		free(tab[j]);
		j++;
	}
	free(tab);
	return (1);
}
