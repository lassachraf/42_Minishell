/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 20:58:27 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/09 13:28:34 by alassiqu         ###   ########.fr       */
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

void	printAST(t_node *node, int x, t_type type)
{
	t_type	tmp;
	t_redir	*new;
	t_node	*temp = node;

	if (!temp)
		return ;
	tmp = ERROR;
	if (temp->type == STRING_NODE)
	{
		print_root(type, x);
		while (temp->data.cmd)
		{
			printf("'%s' ", (char *)temp->data.cmd->content);
			temp->data.cmd = temp->data.cmd->next;
		}
		printf("\n");
	}
	else if (temp->type == PAIR_NODE)
	{
		print_root(type, x);
		if (temp->data.pair.type == PIPE)
		{
			printf("\n*** PIPE ***\n");
			tmp = PIPE;
		}
		else if (temp->data.pair.type == OR)
		{
			printf("\n*** OR ***\n");
			tmp = OR;
		}
		else if (temp->data.pair.type == AND)
		{
			printf("\n*** AND ***\n");
			tmp = AND;
		}
		else if (temp->data.pair.type == L_PAREN)
		{
			printf("L_PAREN\n");
			tmp = L_PAREN;
		}
		// if(node->data.pair.type <= 3)
		// {
		printAST(temp->data.pair.left, 1, tmp);
		printAST(temp->data.pair.right, 0, tmp);
		// }
		// else
		// {
		//     printAST(node->data.pair.left, 1 , tmp);
		//     printAST(node->data.pair.right, 0 , tmp);
		// }
	}
	else if (temp->type == REDIR_NODE)
	{
		print_root(type, x);
		while (temp->data.redir)
		{
			new = temp->data.redir->content;
			printf("REDIR NODE , name: '%s'\n", new->file);
			while (new->cmd)
			{
				printf("'%s' ", (char *)new->cmd->content);
				new->cmd = new->cmd->next;
			}
			printf("\n");
			temp->data.redir = temp->data.redir->next;
		}
	}
	else if (temp->type == ERROR_NODE)
	{
		printf("add '%p', -ERROR -------> '%s", temp, temp->data.error);
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

void	increment_shlvl()
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
	g_minishell->gc = NULL;
	g_minishell->dq_flag = 0;
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
		ft_env(g_minishell->our_env);
	}
	add_env_var(g_minishell->our_env, "?", "0");
	set_as_invisible(g_minishell->our_env, "?");
	set_as_unexported(g_minishell->our_env, "?");
	set_env_var(g_minishell->our_env, "_", "]");
	set_as_unexported(g_minishell->our_env, "_");
	return (signals(), 1);
}

int	get_exit_status()
{
	char	*value;
	int		exit_status;

	value = get_env_var(g_minishell->our_env, "?");
	exit_status = ft_atoi(value);
	return (exit_status);
}

void	ft_readline()
{
	int	exit_status;

	exit_status = 0;
	g_minishell->line = readline(ORANGE PROMPT RESET);
	gc_add(g_minishell, g_minishell->line);
	if (!g_minishell->line)
	{
		if (g_minishell->our_env)
			exit_status = get_exit_status();
		ft_putstr_fd("exit\n", 1);
		clear_env(g_minishell->our_env);
		gc_free_all(g_minishell);
		free(g_minishell);
		exit(exit_status);
	}
	if (g_minishell->line[0])
	{
		add_history(g_minishell->line);
	}
}

int	main(int ac, char **av, char **env)
{
	(void)ac, (void)av;
	if (!init_minishell(env))
		return (1);
	while (1)
	{
		ft_readline();
		g_minishell->tokens = tokenizer();
		if (!g_minishell->tokens || syntax() == -1)
			continue ;
		g_minishell->ast = parsing();
		if (!g_minishell->ast)
			continue ;
		executer(g_minishell->ast);
		while(waitpid(-1, NULL, 0) != -1);
		gc_free_all(g_minishell);
		dup2(g_minishell->stdout, 1);
		dup2(g_minishell->stdin, 0);
	}
	gc_free_all(g_minishell);
	clear_env(g_minishell->our_env);
	free(g_minishell);
	return (0);
}
