/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:11:26 by baouragh          #+#    #+#             */
/*   Updated: 2024/07/09 13:57:27 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**get_env_paths(char **env)
{
	char	**res;

	if (!env)
		return (NULL);
	while (*env && !ft_strnstr(*env, "PATH", 5))
		env++;
	if (!*env)
		return (NULL);
	res = ft_split(*env, ':');
	check_split(res,*env);
	ft_memmove(*res, ft_strchr(*res, '/'), ft_strlen(*res));
	return (res);
}

char	*get_fullpath(char *argv, char **env)
{
	int		paths_num;
	char	**cmd;
	char	**paths;
	char	*fullpath;
	int		i;

	i = 0;
	fullpath = NULL;
	paths = get_env_paths(env);
	paths_num = strings_count(paths);
	cmd = ft_split(argv, ' ');
	if(!cmd)
		return(free(paths),NULL);
	if (!(access(*cmd, F_OK)))
	{
		if ((*argv == '/' || *argv == '.') && !access(*cmd, X_OK))
			return (founded_cmd(argv, paths, cmd));
	}
	else if (*argv != '/' && *argv != '.')
	{
		while (paths_num-- > 0 && !fullpath)
			fullpath = add_slash_cmd(paths[i++], *cmd);
	}
	return (free_double(cmd), free_double(paths), fullpath);
}

int	check_cmd(char *argv, char **env)
{
	char	*cmd;
    int     check;

    check = 0;
	cmd = get_fullpath(argv, env);
	if (!cmd && *argv == '.') // ./
		cmd = get_command(argv);
	if (*argv != '\0' && (*argv == '/' || *argv == '.') && access(cmd, F_OK))
	{
		print_err("badashell: no such file or directory:", argv);
		check = 127;
	}
	else if(access(cmd, F_OK) && argv[ft_strlen(argv) - 1] == '/')
	{
		print_err("badashell: Is a directory : ", argv);
		check = 126;
	}
	else if (*argv != '\0' && access(cmd, F_OK))
	{
		print_err("badashell: Command not found: ", argv);
		check = 127;
	}
	else if (*argv != '\0' && access(cmd, X_OK))
	{
		print_err("badashell: permission denied: ", argv);
		check = 126;
	}
	free(cmd);
    return (check);
}

void	call_execev(char **env, char *argv , char **cmd)
{
	char	*founded_path;

	check_split(cmd, argv);
	founded_path = get_fullpath(argv, env);
	execve(founded_path, cmd, env);
	if(!ft_strncmp(founded_path, argv, ft_strlen(argv) + ft_strlen(founded_path)))
		print_err("bash: Is a directory : ",founded_path);
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
