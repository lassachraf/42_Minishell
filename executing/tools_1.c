/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:11:26 by baouragh          #+#    #+#             */
/*   Updated: 2024/08/09 10:38:14 by alassiqu         ###   ########.fr       */
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

bool	get_path_data(char *argv, char ***paths, char ***cmd, int *paths_num)
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

int	check_for_whitespaces(char *argv)
{
	int	i;

	i = 0;
	while (argv[i])
	{
		if (ft_isspace(argv[i]))
			return (1);
		i++;
	}
	return (0);
}

char	*get_fullpath(char *argv)
{
	int		paths_num;
	char	**cmd;
	char	**paths;
	char	*fullpath;
	int		i;

	if (!argv || !*argv || check_for_whitespaces(argv))
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
		ft_putstr_fd(RED"badashell$ : filename argument required\n", 2);
		ft_putstr_fd(". : usage: . filename [arguments]\n"RESET, 2);
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

	founded_path = get_fullpath(argv);
	printf(RED);
	execve(founded_path, cmd, env);
	printf(RESET);
	print_err("EXEVE FAILED ", NULL);
}
