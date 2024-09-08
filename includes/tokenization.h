/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 14:04:32 by alassiqu          #+#    #+#             */
/*   Updated: 2024/09/08 18:18:31 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZATION_H
# define TOKENIZATION_H

# include <stdbool.h>

typedef enum e_type
{
	WORD,
	PIPE,
	OR,
	AND,
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
	bool			avoid;
	int				quoted;
	int				quoted_af;
	bool			hd_expand;
	bool			wd_expand;
	int				next_space;
	struct s_token	*prev;
	struct s_token	*next;
}					t_token;

/* Tokenizer_free */

// Function that return if the character is a quote or not.
int					is_quote(char c);

// Function that append quote token to the list of token.
int					add_quote(t_token **tokens, char **line);

// Function that clear list of tokens.
void				clear_token(t_token **tokens);

/* Tokenizer_helpers */

// Function that create and return a new token node.
t_token				*new_token(char *value, t_type type, int wd, int quoted);

// Function that add a new token to the list of tokens.
void				add_token_back(t_token **tokens, t_token *new_token);

// Function that append the separator to the list of tokens.
int					append_separator(t_token **tokens, char **line,
						t_type type);

// Function that append other tokens to the list of tokens.
int					append_identifier(t_token **tokens, char **line);

// Function that append the space to the list of tokens.
int					append_space(t_token **tokens, char **line);

/* Tokenizer_utils */

// Function that checks if the character is a separator.
int					is_separator(char *s);

// Function that check for a special case.
int					special_case(t_token *prev, t_token *curr, t_token *next);

// Function that removes a token from the list of tokens.
void				remove_token(t_token **head, t_token *token);

// Function that check if there's a close of quote or not.
int					check_for_quote_close(char **line, int c);

/* Tokenizer */

// The main function that tokenizes the input string.
t_token				*tokenizer(void);

/* Tokens_helpers */

// Function that removes all quotes from the list of tokens.
void				remove_quotes(t_token **tokens);

// Function that removes all whitespaces from the list of tokens.
void				remove_whitespaces(t_token **tokens);

#endif /* TOKENIZATION_H */