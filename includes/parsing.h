/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 09:42:47 by alassiqu          #+#    #+#             */
/*   Updated: 2024/08/04 01:43:44 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# define LEFT 0
# define RIGHT 1
# include "../libft/libft.h"
# include "tokenization.h"

typedef enum e_node
{
	PAIR_NODE,
	CHAR_NODE,
	REDIR_NODE,
	STRING_NODE
}						t_node_type;

typedef struct s_node	t_node;

typedef struct s_redir
{
	t_type				type;
	int					mode;
	int					fd;
	int					hd_expand;
	char				*file;
	bool				is_block;
	t_list				*cmd;
	t_node				*node;
}						t_redir;

typedef struct s_pair_value
{
	t_type				type;
	t_node				*left;
	t_node				*right;
	bool				is_block;
}						t_pair_value;

typedef union u_node_value
{
	t_pair_value		pair;
	char				char_value;
	t_list				*cmd;
	t_list				*redir;
}						t_node_value;

struct					s_node
{
	t_node_type			type;
	t_node_value		data;
};

/* Nodes */

// Function that create a new character node.
t_node				*char_node_new(char c);

// Function that create a pair of nodes.
t_node				*pair_node_new(t_node *left, t_node *right, t_type type);

// Function that create a new string node.
t_node				*string_node_new(t_list *list);

// Function that create a new redirection node.
t_node				*redir_node_new(t_list *red_list);

/* Parsing */

// The main function for parsing the input and return the AST.
t_node				*parsing(void);

// Function that parse a block or a sequence.
t_node				*parse_block(t_token **tokens);

// Function that parse "pipe" expression.
t_node				*parse_pipe(t_token **tokens);

// Function that parse "or" expression.
t_node				*parse_or(t_token **tokens);

// Function that parse "and" expression.
t_node				*parse_and(t_token **tokens);

// Function that parse a command.
t_node				*parse_cmd(t_token **tokens);

#endif /* PARSER_H */