/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 21:25:20 by alassiqu          #+#    #+#             */
/*   Updated: 2024/06/27 13:58:58 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_command(char *argv)
{
	char	*cmd;
	int		i;

	i = 0;
	while (argv[i] != ' ' && argv[i] != '\0')
		i++;
	cmd = malloc(i + 1);
	if (!cmd)
	{
		perror("malloc");
		return (NULL);
	}
	ft_memmove(cmd, argv, i);
	cmd[i] = '\0';
	return (cmd);
}

char	*add_slash_cmd(char *path, char *cmd)
{
	char	*a_path;
	char	*fullpath;

	a_path = (ft_strjoin(path, "/"));
	fullpath = ft_strjoin(a_path, cmd);
	if (!access(fullpath, X_OK))
		return (free(a_path), fullpath);
	else
		return (free(fullpath), free(a_path), NULL);
}

int	print_err(char *message, char *word)
{
	ft_putstr_fd(RED, 2);
	ft_putstr_fd(message, 2);
	ft_putstr_fd(word, 2);
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
		exit (EXIT_FAILURE);
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

static char	*founded_cmd(char *argv, char **paths, char **cmd)
{
	char	*fullpath;

	fullpath = get_command(argv);
	if (!fullpath)
	{
		free_double(paths);
		free_double(cmd);
		exit(EXIT_FAILURE);
	}
	return (free_double(paths), free_double(cmd), fullpath);
}

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

	if (*argv == '\0')
		return (strdup(""));
	i = 0;
	fullpath = NULL;
	paths = get_env_paths(env);
	paths_num = strings_count(paths);
	cmd = ft_split(argv, ' ');
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

    check = 1;
	cmd = get_fullpath(argv, env);
	if (!cmd && *argv == '.')
		cmd = get_command(argv);
	if (*argv != '\0' && (*argv == '/' || *argv == '.')
		&& access(cmd, F_OK))
		check = print_err("badashell: no such file or directory:", argv);
	else if (*argv != '\0' && access(cmd, F_OK))
		check = print_err("badashell: command not found: ", argv);
	else if (*argv != '\0' && access(cmd, X_OK))
		check = print_err("badashell: permission denied: ", argv);
	free(cmd);
    return (check);
}

void	call_execev(char **env, char *argv , char **cmd)
{
	char	*cat[2];
	char	*founded_path;

	check_split(cmd, argv);
	founded_path = get_fullpath(argv, env);
	if (!founded_path)
	{
		free_double(cmd);
		exit(EXIT_FAILURE);
	}
	cat[0] = "cat";
	cat[1] = NULL;
	if (*argv == '\0')
	{
		free(founded_path);
		free_double(cmd);
		execve(get_fullpath("cat", env), cat, env);
	}
	else
    {
		execve(founded_path, cmd, env);
    }
	print_err("badashell: command not found: ", "cat");
	exit(EXIT_FAILURE);
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

int  env_size(t_env *env)
{
    int size;
    
    size = 0;
    while(env)
    {
        if(env->visible)
            size++;
        env = env->next;
    }
    return(size);
}

void print_double(char ** argv)
{
    int i = 0;

    while(argv[i])
        printf("%s\n",argv[i++]);
}

char **env_to_envp(t_env *env)
{
    char **argv;
    char    *tmp;
    int size;
    int i;

    i = 0;
    size = env_size(env);
    argv = malloc(sizeof(char *) * (size + 1));
    if(!argv)
        return(NULL);
    gc_add(g_minishell, argv);
    while(i < size)
    {
        tmp = ft_strjoin(env->key, "=");
        argv[i] = ft_strjoin(tmp ,env->value);
        if(!argv[i])
            return(gc_free_all(g_minishell), NULL);
        gc_add(g_minishell, argv[i]);
        free(tmp);
        env = env->next;
        i++;
    }
    argv[i] = NULL;
    return(argv);
}

char **list_to_argv(t_list *list)
{
    char **argv;
    int size;
    int i;
    int len;

    i = 0;
    size = ft_lstsize(list);
    argv = malloc(sizeof(char *) * (size + 1));
    if(!argv)
        return(gc_free_all(g_minishell), NULL);
    gc_add(g_minishell, argv);
    while(i < size)
    {
        len = ft_strlen(list->content) + 1;
        argv[i] = malloc(sizeof(char) * len);
        if(!argv[i])
            return(gc_free_all(g_minishell), NULL);
        gc_add(g_minishell, argv[i]);
        ft_memmove(argv[i], list->content, len);
        list = list->next;
        i++;
    }
    argv[i] = NULL;
    return(argv);
}

void do_cmd(void)
{
    int id;
    char **cmd;
    char **env;

    cmd = list_to_argv(g_minishell->ast->data.cmd);
    if(!cmd)
        return;
    env = env_to_envp(g_minishell->our_env);
    if(!env)
        return;
    id = check_cmd(*cmd, env);
    if(id)
    {
        id = fork();
        if (!id)
            call_execev(env, *cmd , cmd);
        else
            wait(NULL);
    }
}

void    executer(void)
{
    t_node *node;

    node = g_minishell->ast;
	// while (node)
	// {
		if (node->type == STRING_NODE)
		{
			if (ft_is_builtin(node->data.cmd->content))
				execute_builtins(g_minishell, list_to_argv(g_minishell->ast->data.cmd));
			else
				do_cmd();
		}
		// else if(node->type == PAIR_NODE)
		// {
		//         printAST(node->data.pair.left, 1 , tmp);
		//         printAST(node->data.pair.right, 0 , tmp);
		// }
		// else if (node->type == REDIR_NODE)
		// {
		//     while(node->data.redir)
		//     {
		//         t_redir *new = node->data.redir->content;
		//         printf("REDIR NODE , name: '%s'\n",new->file);
		//         while (new->cmd)
		//         {
		//             printf("'%s' ", (char*)new->cmd->content);
		//             new->cmd = new->cmd->next;
		//         }
		//         printf("\n");
		//         node->data.redir = node->data.redir->next;
		//     }
		// }
		// else if(node->type == ERROR_NODE)
		// {
		//     printf("add'%p', -ERROR -------> '%s",node ,node->data.error);
		// }
	// }
    
	
}
