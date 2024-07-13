/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 15:33:43 by baouragh          #+#    #+#             */
/*   Updated: 2024/07/13 15:48:05 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	execute_redires(t_list *red_list) // here_Doce 16 // 16 // 
{
	int fd_input;
	int fd_output;

	if(!open_and_set(red_list))
		return;
	fd_input = input_to_dup(red_list); // < <<
	fd_output = output_to_dup(red_list); // > >>
	if(fd_input > 0)
		dup2(fd_input, 0);
	if(fd_output > 0)
		dup2(fd_output, 1);
	run_doc_cmd(red_list);
}

void execute_cmd(t_node *node)
{
	int id;

	id = 0;
	if (ft_is_builtin(node->data.cmd->content))
        execute_builtins(g_minishell, list_to_argv(node->data.cmd));
    else
	{
		id = fork();
		if(!id)
		{
            do_cmd(node);
		}
	}
}

void execute_and_or(t_node *node)
{
	if (node->data.pair.type == OR)
	{
		executer(node->data.pair.left);
		wait_and_get();
		if(g_minishell->exit_s)
		{
			dup2(g_minishell->stdin, 0);
			dup2(g_minishell->stdout, 1);
			executer(node->data.pair.right);
		}
	}
	else if (node->data.pair.type == AND)
	{
		executer(node->data.pair.left);
		wait_and_get();
		if(!g_minishell->exit_s)
		{
			dup2(g_minishell->stdin, 0);
			dup2(g_minishell->stdout, 1);
			executer(node->data.pair.right);
		}
	}
}

void execute_pair(t_node *node) // ls > a | cat a
{
	int	pfd[2];
	int id;

	if(node->data.pair.type == PIPE) // (cat -e && ps) | (cat -n && true)
	{
		open_pipe(pfd);
		if(node->data.pair.left->type != STRING_NODE && node->data.pair.left->type != PIPE) // LEEEFT side of PIPE CASE 
		{
			if (node->data.pair.left->data.pair.type == AND)
			{
				id = fork();
				if(!id)
				{
					close(pfd[0]); // read from pipe [pipe]
					dup2(pfd[1],1); // write to [pipe]
					executer(node->data.pair.left->data.pair.left); // RUN LEFT OF AND
					wait_and_get();
					if(!g_minishell->exit_s) 
						executer(node->data.pair.left->data.pair.right); // RUN RIGHT OF AND IF LEFT TRUE
					while(waitpid(-1, NULL, 0)!= -1);
					fprintf(stderr, "=>>> %d\n", g_minishell->exit_s);

					exit(g_minishell->exit_s);
				}
				dup2(pfd[0],0); // read from pipe [pipe]
			}
			else if(node->data.pair.left->data.pair.type == OR) // OR CASE
			{
				id = fork();
				if(!id)
				{
					close(pfd[0]); // read from pipe [pipe]
					dup2(pfd[1],1); // write to [pipe]
					executer(node->data.pair.left->data.pair.left); // RUN LEFT OF OR
					wait_and_get();
					if(g_minishell->exit_s)
						executer(node->data.pair.left->data.pair.right); // RUN RIGHT OF OR if LEFT FALSE
					while(waitpid(-1, NULL, 0)!= -1)
					fprintf(stderr, "=>>> %d\n", g_minishell->exit_s);

					exit(g_minishell->exit_s);
				}
				dup2(pfd[0],0); // read from pipe [pipe]
			}
			else if(node->data.pair.left->type == REDIR_NODE)
			{
				executer(node->data.pair.left);
				dup2(g_minishell->stdout, 1);
				dup2(g_minishell->stdin, 0);
			}
			else
				executer(node->data.pair.left);
		}		
		else if(node->data.pair.left->type == STRING_NODE)
			do_pipe(node->data.pair.left , 0 , pfd); // close(pfd[1]);
		
		if(node->data.pair.right->type != STRING_NODE) // RIGHT side of PIPE CASE   //   | (cat && do) 
		{
			if (node->data.pair.right->data.pair.type == AND)
			{
				id = fork();
				if(!id)
				{
					close(pfd[0]); // read from pipe [pipe]
					close(pfd[1]);
					executer(node->data.pair.right->data.pair.left); // RUN LEFT OF AND
					wait_and_get();
					if(!g_minishell->exit_s)
						executer(node->data.pair.right->data.pair.right); // RUN RIGHT OF AND IF LEFT TRUE
					while(waitpid(-1, NULL, 0)!= -1);
					fprintf(stderr, "=>>> %d\n", g_minishell->exit_s);

					exit(g_minishell->exit_s);
				}
				close(pfd[0]);
			}
			else if(node->data.pair.right->data.pair.type == OR) // OR CASE
			{
				id = fork();
				if(!id)
				{
					close(pfd[0]); // read from pipe [pipe]
					close(pfd[1]);
					executer(node->data.pair.right->data.pair.left); // RUN LEFT OF OR
					wait_and_get();
					if(g_minishell->exit_s)
						executer(node->data.pair.right->data.pair.right); // RUN RIGHT OF OR if LEFT FALSE
					while(waitpid(-1, NULL, 0)!= -1);
					fprintf(stderr, "=>>> %d\n", g_minishell->exit_s);
					exit(g_minishell->exit_s);
				}
				close(pfd[0]);
			}
			else
				executer(node->data.pair.right);
		}
		else
			do_pipe(node->data.pair.right , 1 , pfd);
	}
	else
		execute_and_or(node);
	close(0);
	close(1);
	close(pfd[1]);
	close(pfd[0]);
}

void    executer(t_node *node) // ls | wc | cat && ps
{
	if (!node)
		return;
    if (node->type == STRING_NODE) // leaf
		execute_cmd(node);
	else if(node->type == PAIR_NODE) // pair : | && ||
		execute_pair(node);
    else if (node->type == REDIR_NODE) // leaf
		execute_redires(node->data.redir);
}


/*
			// else if(node->data.pair.right->data.pair.type == PIPE) // PIPE CASE
			// {
			// 	if(node->data.pair.right->data.pair.left->type != STRING_NODE)
			// 		executer(node->data.pair.right->data.pair.left);
			// 	else
			// 		do_pipe(node->data.pair.right->data.pair.left, 0, pfd);
			// 	if(node->data.pair.right->data.pair.right->type != STRING_NODE)
			// 		executer(node->data.pair.right->data.pair.right);
			// 	else
			// 		do_pipe(node->data.pair.right->data.pair.right, 1, pfd); // do last one
			// }
*/