/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 21:25:20 by alassiqu          #+#    #+#             */
/*   Updated: 2024/06/26 10:00:07 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

    id = fork();
    if (!id)
        execve("/usr/bin/ls", list_to_argv(g_minishell->ast->data.cmd), env_to_envp(g_minishell->our_env));
    else
    {
        wait(NULL);
    }
}

void    executer(void) // execve( char *path, char **argv, char **envp);
{
    t_node *node;

    node = g_minishell->ast;
    if (node->type == STRING_NODE)
    {
        if (ft_is_builtin(node->data.cmd->content))
        {
            printf("Yes its a builtin\n");
            execute_builtins(g_minishell, list_to_argv(g_minishell->ast->data.cmd));
        }
        else
            do_cmd();
    }
	// else if(node->type == PAIR_NODE)
	// {
    //         printAST(node->data.pair.left, 1 , tmp);
    //         printAST(node->data.pair.right, 0 , tmp);
    // }
//     else if (node->type == REDIR_NODE)
//     {
//         while(node->data.redir)
//         {
//             t_redir *new = node->data.redir->content;
//             printf("REDIR NODE , name: '%s'\n",new->file);
//             while (new->cmd)
//             {
//                 printf("'%s' ", (char*)new->cmd->content);
//                 new->cmd = new->cmd->next;
//             }
//             printf("\n");
//             node->data.redir = node->data.redir->next;
//         }
//     }
//     else if(node->type == ERROR_NODE)
//     {
//         printf("add'%p', -ERROR -------> '%s",node ,node->data.error);
//     }
}