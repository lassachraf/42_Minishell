/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 15:33:43 by baouragh          #+#    #+#             */
/*   Updated: 2024/07/05 15:28:16 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	open_pipe(int *pfd)
{
	if (pipe(pfd))
	{
		perror("pipe:");
		return;
	}
}

int	dup_2(int old_fd, int new_fd)
{
	if (dup2(old_fd, new_fd) < 0)
		return (-1);
	close(old_fd);
	return (0);
}


void	fd_duper( int *pfd , int mode)
{
	if (mode == 1)
	{
		close(pfd[1]);
		close(pfd[0]);
	}
	else
	{
		close(pfd[0]);
		if (dup_2(pfd[1], 1)) // 
			exit(EXIT_FAILURE);
	}
}

char	*get_command(char *argv)
{
	char	*cmd;
	int		i;

	i = 0;
	while (argv[i] != ' ' && argv[i] != '\0')
		i++;
	cmd = malloc((i + 1) * sizeof(char));
	if (!cmd)
	{
		perror("malloc");
		return (NULL);
	}
	gc_add(g_minishell, cmd);
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
		return;
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
		return(NULL);
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
	if (!cmd && *argv == '.')
		cmd = get_command(argv);
	if (*argv != '\0' && (*argv == '/' || *argv == '.')
		&& access(cmd, F_OK))
	{
		print_err("badashell: no such file or directory:", argv);
		check = 127;
	}
	else if (*argv != '\0' && access(cmd, F_OK))
	{
		print_err("badashell: command not found: ", argv);
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
	print_err("execve failed !!\n", NULL);
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
    {
        printf("%s\n",argv[i]);
        i++;
    }
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
	if(!list)
		return(NULL);
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
        ft_memmove(argv[i++], list->content, len);
        list = list->next;
    }
    argv[i] = NULL;
    return(argv);
}

void do_cmd(t_node *ast)
{
    int id;
    char **cmd;
    char **env;

    cmd = list_to_argv(ast->data.cmd);
    if(!cmd)
        return;
    env = env_to_envp(g_minishell->our_env);
    if(!env)
		return;
	id = check_cmd(*cmd, env);
	if(!id)
		call_execev(env, *cmd , cmd);
	exit(id);
}

/*
	if mode == 0 it means a reagular dup of stdout to pipe write end pfd[1]
	else 
		it means thats cmd its last comd and dup should be to stdout or fd.
*/

void wait_and_get(void)
{
	char *exit;

	wait(&g_minishell->exit_s);
	if (WIFEXITED(g_minishell->exit_s))
		g_minishell->exit_s = WEXITSTATUS(g_minishell->exit_s);
	exit = ft_itoa(g_minishell->exit_s);
	if(!exit)
		return(print_errors("ERROR WITH FT_ITOA\n"));
	set_env_var(g_minishell->our_env, "?", exit);
	free(exit);
}

void open_redir(t_redir *redir)
{
	redir->fd = open(redir->file,redir->mode, 0644);
}


void do_pipe(t_node *cmd , int mode) // ls | cat | cat -e
{
	int	id;
	int	pfd[2];

	open_pipe(pfd);
	id = fork();
	if (id < 0)
	{
		print_err("pipex: error occuerd with fork!", NULL);
		return;
	}
	if (id == 0)
	{
		fd_duper(pfd, mode);
		do_cmd(cmd);
	}
	else
	{
		close(pfd[1]);
		dup_2(pfd[0], 0);
		if(mode)
			wait_and_get();
	}
}

int do_here_docs(t_list *red_list)
{
	t_redir *new ;

	while(red_list)
	{
		unlink("/var/tmp/tmp.txt");
		new = red_list->content;
		if(new->type == LL_REDIR)
		{
			new->fd = here_doc(new->file);
			if(new->fd < 0)
				return(0);
		}
		red_list = red_list->next;
	}
	return(1);
}

int input_to_dup(t_list *red_list) // < <<
{
	t_redir *new ;
	int fd;
	
	fd = -1;
	while(red_list)
	{
		new = red_list->content;
		if(new->type == L_REDIR || new->type == LL_REDIR)
			fd = new->fd;
		red_list = red_list->next;
	}
	return (fd);
}

int output_to_dup(t_list *red_list) // > >>
{
	t_redir *new ;
	int fd;
	
	fd = -1;
	while(red_list)
	{
		new = red_list->content;
		if(new->type == R_REDIR || new->type == RR_REDIR)
			fd = new->fd;
		red_list = red_list->next;
	}
	return (fd);
}

void open_and_set(t_list *red_list)
{
    t_redir *new ;
	while(red_list) // linked list of reds
    {
		new = red_list->content;
        // printf("REDIR NODE , name: '%s'\n",new->file);
		if(new->type != LL_REDIR)
			open_redir(new);
        red_list = red_list->next;
    }
}

void    executer(t_node *node)
{
	int id;
	int fd_input;
	int fd_output;

	if (!node)
		return;
    if (node->type == STRING_NODE)
    {
        if (ft_is_builtin(node->data.cmd->content))
            execute_builtins(g_minishell, list_to_argv(node->data.cmd));
        else
		{
			id = fork();
			if(!id)
            	do_cmd(node);
			else
				wait_and_get();
		}
    }
	else if(node->type == PAIR_NODE)
	{
		if(node->data.pair.type == PIPE)
		{
			do_pipe(node->data.pair.left , 0); 
			if(node->data.pair.right->type != STRING_NODE)
				executer(node->data.pair.right);
			else
				do_pipe(node->data.pair.right, 1);
		}
		else if (node->data.pair.type == OR)
		{
			executer(node->data.pair.left);
			if(g_minishell->exit_s)
			{
				dup2(g_minishell->stdin, 0);
				executer(node->data.pair.right);
			}
		}
		else if (node->data.pair.type == AND)
		{
			executer(node->data.pair.left);
			if(!g_minishell->exit_s)
			{
				dup2(g_minishell->stdin, 0);
				executer(node->data.pair.right);
			}
		}
    }
    else if (node->type == REDIR_NODE) //leaf // ls -a < input1 << here1 << here2 < input2 < input3 << here3 > output1 >> output2 -k
    {
		// do here_docs first
		t_list *last;
		t_redir *new;
		
		if (do_here_docs(node->data.redir) == 0)
			return (print_errors("ERROR ACCURE WITH HERE_DOC\n"));
		open_and_set(node->data.redir);
		fd_input = input_to_dup(node->data.redir);
		fd_output = output_to_dup(node->data.redir);
		if(fd_input > 0)
			dup2(fd_input, 0);
		if(fd_output > 0)
			dup2(fd_output, 1);
		last = ft_lstlast(node->data.redir);
		new = last->content;
		if(new->cmd)
			executer(string_node_new(new->cmd));
    }
//     else if(node->type == ERROR_NODE)
//     {
//         printf("add'%p', -ERROR -------> '%s",node ,node->data.error);
//     }
}
