/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 14:04:32 by alassiqu          #+#    #+#             */
/*   Updated: 2024/06/06 16:37:36 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZATION_H
# define TOKENIZATION_H

# include <stdbool.h>

typedef enum e_type
{
	WORD,
	PIPE,
	AND,
	OR,
	LL_REDIR,
	RR_REDIR,
	L_REDIR,
	R_REDIR,
	END,
	L_PAREN,
	R_PAREN,
	S_QUOTE,
	D_QUOTE,
	ASTERISK,
	DOLLAR,
	AMPERSAND,
	WHITESPACE,
	ERROR
}					t_type;

typedef struct s_token
{
	char			*value;
	t_type			type;
	struct s_token	*prev;
	struct s_token	*next;
}					t_token;

/* Utils */
/* Function that checks if the character is either 
a single quote or a double quote. */
int					is_special(char c);
// Function that checks if the character is a separator.
int					is_separator(char *s);
// Function that skips the whitespaces.
void				skip_spaces(char **line);
// Function that skips the quotes.
bool				skip_quotes(char *line, size_t *i);
// Function that prints in case of error.
void				print_quote_err(char c);

/* Helpers */
// Function that create and return a new token node.
t_token				*new_token(char *value, t_type type);
// Function that add a new token to the list of tokens.
void				add_token_back(t_token **tokens, t_token *new_token);
// Function that append the separator to the list of tokens.
int					append_separator(t_token **tokens, char **line,
						t_type type);
// Function that append other tokens to the list of tokens.
int					append_identifier(t_token **tokens, char **line);
// Function that append the spaces.
int					append_space(t_token **tokens, char **line);

/* Free */
void				clear_token(t_token **tokens);

#endif /* TOKENIZATION_H */