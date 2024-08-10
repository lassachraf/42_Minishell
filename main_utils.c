/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 02:03:45 by alassiqu          #+#    #+#             */
/*   Updated: 2024/08/09 09:15:21 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

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
			fprintf(stderr," |\n");

      
        }
        else if (root->data.pair.type == OR)
        {
			fprintf(stderr," ||\n");

        } 
        else if (root->data.pair.type == AND)
        {
			fprintf(stderr," &&\n");
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
            fprintf(stderr,"'%s' ", (char*)list->content);
            list = list->next;
        }
        fprintf(stderr,"\n");
	}
	else if (root->type == REDIR_NODE) 
	{
		t_list *lst;
		lst = root->data.redir;
		while(lst)
        {
			t_list *list;
            t_redir *new = lst->content;
            fprintf(stderr,"REDIR NODE , name: '%s' ",new->file);
			list = new->cmd;
            while (list)
            {
                fprintf(stderr,"'%s' ", (char*)list->content);
                list = list->next;
            }
            fprintf(stderr," ");
            lst = lst->next;
        }
        fprintf(stderr,"\n");
	}   
}


void	print_tokens(t_token *tokens)
{
	t_token	*token;

	token = tokens;
	while (token)
	{
		printf("value => '%s'\n", token->value);
		printf("type => '%u'\n", token->type);
		token = token->next;
	}
}

void	clean_and_set(void)
{
	char	*exit_stat;

	dup2(g_minishell->stdout, 1);
	dup2(g_minishell->stdin, 0);
	gc_free_all(g_minishell);
	unlink_docs(g_minishell->docs);
	exit_stat = ft_itoa(g_minishell->exit_s);
	set_env_var(g_minishell->our_env, "?", exit_stat);
	free(exit_stat);
}

void	exit_and_set(void)
{
	char	*exit;

	exit = ft_itoa(g_minishell->exit_s);
	set_env_var(g_minishell->our_env, "?", exit);
	free(exit);
}

int	wait_last(void)
{
	int		fail;
	int		x;

	fail = -1;
	fail = waitpid(g_minishell->last_child, &x, 0);
	if (fail == -1)
		return (exit_and_set(), -1);
	if (x == 131)
	{
		g_minishell->exit_s = x;
		ft_putstr_fd("Quit (core dumped)\n", 2);
		return (exit_and_set(), -1);
	}
	if (WIFEXITED(x))
		g_minishell->exit_s = WEXITSTATUS(x);
	if (g_minishell->exit_s == 130 && x == 2)
		ft_putstr_fd("\n", 2);
	return (exit_and_set(), -1);
}
