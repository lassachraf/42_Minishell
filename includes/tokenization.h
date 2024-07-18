/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 14:04:32 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/17 22:18:34 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZATION_H
# define TOKENIZATION_H

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
	WORD_QUOTE,
	ERROR
}					t_type;

typedef struct s_token
{
	char			*value;
	t_type			type;
	int				next_space;
	struct s_token	*prev;
	struct s_token	*next;
}					t_token;

/* Tokenizer_utils */

// Function that checks if the character is a quote.
int					is_special(char c);

// Function that checks if the character is a separator.
int					is_separator(char *s);

// Function that skips the whitespaces.
void				skip_spaces(char **line);

// Function that check for a special case.
int					special_case(t_token *prev, t_token *curr, t_token *next);

/* Tokenizer_helpers */

// Function that create and return a new token node.
t_token				*new_token(char *value, t_type type);

// Function that add a new token to the list of tokens.
void				add_token_back(t_token **tokens, t_token *new_token);

// Function that append the separator to the list of tokens.
int					append_separator(t_token **tokens, char **line,
						t_type type);

// Function that append other tokens to the list of tokens.
int					append_identifier(t_token **tokens, char **line);

// Function that append the space to the list of tokens.
int					append_space(t_token **tokens, char **line);

/* Tokens_helpers */

// Function that removes a token from the list of tokens.
void				remove_token(t_token **head, t_token *token);

// Function that removes all whitespaces from the list of tokens.
void				remove_whitespaces(t_token **tokens);

// Function that removes all quotes from the list of tokens.
void				remove_quotes(t_token **tokens);

/* Tokenizer_free */

// Function that clear list of tokens.
void				clear_token(t_token **tokens);

// Function that specifie the the type of the token.
t_token				*choose_token(char *value, char c);

// Function that append quote token to the list of token.
int					add_quote(t_token **tokens, char **line);

// Function that return if the character is a quote or not.
int					is_quote(char c);

/* Tokenizer */

// The main function that tokenizes the input string.
t_token				*tokenizer(void);

// Function that add a token in the midlle of a list.
void				add_token_middle(t_token **tokens, t_token *new_token, t_token *prev_token);

#endif /* TOKENIZATION_H */