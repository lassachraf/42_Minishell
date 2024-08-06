/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 20:58:27 by alassiqu          #+#    #+#             */
/*   Updated: 2024/08/06 18:03:55 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

t_minishell	*g_minishell;


#define W_GREEN   "\033[32m"      /* Green */
#define W_WHITE   "\033[37m"      /* White */

void	print_ast(const char *prefix,  t_node* root, bool isLeft)
{
	char	*dup;
	char	*join;

    if(!root)
		return ;
	fprintf(stderr,W_GREEN"%s", prefix);
	fprintf(stderr,"%s", (isLeft ? "├──" : "└──" ));
	fprintf(stderr,W_WHITE"");

	if (root->type == PAIR_NODE) {

		if(root->data.pair.type == PIPE)
        {
			fprintf(stderr," | ,is BLOCK : %d\n",root->data.pair.is_block);
      
        }
        else if (root->data.pair.type == OR)
        {
			fprintf(stderr," || ,is BLOCK : %d\n",root->data.pair.is_block);

        } 
        else if (root->data.pair.type == AND)
        {
			fprintf(stderr," && ,is BLOCK : %d\n",root->data.pair.is_block);
        }
		dup = strdup((isLeft ? "│   " : "    "));
		join = ft_strjoin(prefix ,  dup);
    	print_ast(  join ,  root->data.pair.left,  true);
    	print_ast(  join ,   root->data.pair.right, false);	
		free(dup);
		free(join);	
	} 
	else if (root->type == STRING_NODE) 
	{
		t_list *list;
		list = root->data.cmd;
        while (list)
        {
			fprintf(stderr,"list is BLOCK : %d  --> ",list->is_block);
            fprintf(stderr,"'%s' ", (char*)list->content);
            list = list->next;
        }
        fprintf(stderr,"\n");
	}
	else if (root->type == REDIR_NODE) 
	{
		t_list *lst;
		lst = root->data.redir;
		fprintf(stderr,"REDIR_LIST is BLOCK : %d ",lst->is_block);
		while(lst)
        {
			
			t_list *list;
            t_redir *new = lst->content;
            fprintf(stderr,"REDIR NODE , name: '%s' ",new->file);
			fprintf(stderr,"REDIR is BLOCK : %d\n",new->is_block);
			list = new->cmd;
            while (list)
            {
                fprintf(stderr,"'%s' ", (char*)list->content);
                list = list->next;
            }
			if(new->node)
			{
				write(2,"\n",1);
				print_ast("", new->node, false);
			}
            fprintf(stderr," ");
            lst = lst->next;
        }
        fprintf(stderr,"\n");
	}   
}

void	increment_shlvl(void)
{
	char	*shlvl;
	char	*new_shlvl;
	int		tmp;

	shlvl = get_env_var(g_minishell->our_env, "SHLVL");
	if (!shlvl)
		return ;
	tmp = ft_atoi(shlvl) + 1;
	new_shlvl = ft_itoa(tmp);
	gc_add(g_minishell, new_shlvl);
	set_env_var(g_minishell->our_env, "SHLVL", new_shlvl);
}

int	init_minishell(char **env)
{
	g_minishell = malloc(sizeof(t_minishell));
	if (!g_minishell)
		return (0);
	ft_bzero(g_minishell, sizeof(t_minishell));
	g_minishell->stdin = dup(0);
	g_minishell->stdout = dup(1);
	if (env && *env)
	{
		g_minishell->our_env = dup_env(env);
		increment_shlvl();
	}
	else
		g_minishell->our_env = special_dup_env();
	add_env_var(g_minishell->our_env, "?", "0");
	set_as_invisible(g_minishell->our_env, "?");
	set_as_unexported(g_minishell->our_env, "?");
	set_env_var(g_minishell->our_env, "_", "]");
	set_as_unexported(g_minishell->our_env, "_");
	signals();
	return (1);
}

void	ft_readline(void)
{
	int	exit_status;

	exit_status = 0;
	g_minishell->lines++;
	g_minishell->docs = 0;
	g_minishell->line = readline(PROMPT);
	gc_add(g_minishell, g_minishell->line);
	if (!g_minishell->line)
	{
		exit_status = g_minishell->exit_s;
		ft_putstr_fd("exit\n", 1);
		cleanup_minishell();
		exit(exit_status);
	}
	if (g_minishell->line[0])
		add_history(g_minishell->line);
}

int	main(int argc, char **argv, char **env)
{
	(void)argc, (void)argv;
	if (!init_minishell(env))
		return (1);
	while (1)
	{
		signals();
		ft_readline();
		signal(SIGINT, ft_sigint);
		g_minishell->tokens = tokenizer();
		if (!g_minishell->tokens || syntax() == -1)
			continue ;
		g_minishell->ast = parsing();
		if (!g_minishell->ast)
			continue ;
		if (scan_and_set(g_minishell->ast))
			executer(g_minishell->ast);
		dup2(g_minishell->stdout, 1);
		dup2(g_minishell->stdin, 0);
		wait_last();
		while (waitpid(-1, NULL, 0) != -1)
			;
		clean_and_set();
	}
	return (cleanup_minishell(), 0);
}
