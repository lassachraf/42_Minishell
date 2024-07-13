/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 20:58:27 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/13 15:53:21 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

t_minishell	*g_minishell;

void	print_root(t_type type, int x)
{
	if (type == PIPE)
	{
		if (x == 1)
			printf("\nLEFT OF '|' -> ");
		else if (x == 0)
			printf("\nRIGHT OF '|' ----> ");
	}
	else if (type == OR)
	{
		if (x == 1)
			printf("\nLEFT OF '||' ----> ");
		else if (x == 0)
			printf("\nRIGHT OF '||' ----> ");
	}
	else if (type == AND)
	{
		if (x == 1)
			printf("\nLEFT OF '&&' ----> ");
		else if (x == 0)
			printf("\nRIGHT OF '&&' ----> ");
	}
	else if (type == L_PAREN)
	{
		if (x == 1)
			printf("\nLEFT OF '(' ----> ");
		else if (x == 0)
			printf("\nRIGHT OF '(' ----> ");
	}
}

void printAST(t_node* node , int x , t_type type) 
{
    t_type tmp;
	t_list *list;

    if (!node) return;
    tmp  = ERROR;
    if (node->type == STRING_NODE) // leaf
    {
        print_root(type, x);
		list = node->data.cmd;
        while (list)
        {
            printf("'%s' ", (char*)list->content);
            list = list->next;
        }
        printf("\n");
    }
	else if(node->type == PAIR_NODE) // root
	{
		print_root(type, x); // 4832948 , 234234
        if(node->data.pair.type == PIPE)
        {
            printf("------------------->PIPE<----------------------\n");
            tmp = PIPE;
        }
        else if (node->data.pair.type == OR)
        {
            printf("------------------->OR<----------------------\n");
            tmp = OR;
        } 
        else if (node->data.pair.type == AND)
        {
            printf("------------------->AND<----------------------\n");
            tmp = AND;
        }
            printAST(node->data.pair.left, 1 , tmp);
            printAST(node->data.pair.right, 0 , tmp);
    }
    else if (node->type == REDIR_NODE) // leaf
    {
        print_root(type, x);
        while(node->data.redir)
        {
            t_redir *new = node->data.redir->content;
            printf("REDIR NODE , name: '%s'\n",new->file);
            while (new->cmd)
            {
                printf("'%s' ", (char*)new->cmd->content);
                new->cmd = new->cmd->next;
            }
            printf("\n");
			if(new->node)
				printAST(new->node, 121,231);
            node->data.redir = node->data.redir->next;
        }
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
	{
		g_minishell->our_env = special_dup_env();
	}
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
	g_minishell->docs = 0;
	g_minishell->exit_s = 0;
	g_minishell->line = readline(ORANGE PROMPT RESET);
	gc_add(g_minishell, g_minishell->line);
	if (!g_minishell->line)
	{
		exit_status = g_minishell->exit_s;
		ft_putstr_fd("exit\n", 1);
		clear_env(g_minishell->our_env);
		gc_free_all(g_minishell);
		free(g_minishell);
		exit(exit_status);
	}
	if (g_minishell->line[0])
		add_history(g_minishell->line);
}	

int	main(int achraf, char **bader, char **env)
{
	(void)achraf, (void)bader;
	char *exit_stat;
	if (!init_minishell(env))
		return (1);
	while (1)
	{
		signals();
		ft_readline();
		// fprintf(stderr, "-------------> %d\n", g_minishell->exit_s);
		g_minishell->tokens = tokenizer();
		if (!g_minishell->tokens || syntax() == -1)
			continue ;
		g_minishell->ast = parsing();
		if (!g_minishell->ast)
			continue ;
		signal(SIGINT, SIG_IGN);
		if(scan_and_set(g_minishell->ast))
		{
			signal(SIGQUIT, hand);
			signal(SIGINT, hand2);
			executer(g_minishell->ast);
		}
		// fprintf(stderr, "** Before :: `%d` **\n", g_minishell->exit_s);
		while (wait_and_get() != -1)
			;
		// fprintf(stderr, "** After :: `%d` **\n", g_minishell->exit_s);
		gc_free_all(g_minishell);
		dup2(g_minishell->stdout, 1);
		dup2(g_minishell->stdin, 0);
		unlink_docs(g_minishell->docs);
		exit_stat = ft_itoa(g_minishell->exit_s);
		// fprintf(stderr, "-------------> %s\n", exit_stat);
		set_env_var(g_minishell->our_env, "?", exit_stat);
		free(exit_stat);
	}
	cleanup_minishell();
	return (0);
}
