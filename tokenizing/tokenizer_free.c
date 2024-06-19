/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_free.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 19:58:45 by alassiqu          #+#    #+#             */
/*   Updated: 2024/06/06 15:38:16 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_quote(char c)
{
	if (c == '"')
		return (1);
	else if (c == '\'')
		return (2);
	return (0);
}

int	add_string(t_token **tokens, char **line, char quote_char)
{
	t_token	*new;
	char	*value;
	size_t	i;

	i = 0;
	while ((*line)[i] && (*line)[i] != quote_char)
		i++;
	if (i == 0)
		return (1);
	value = ft_substr(*line, 0, i);
	new = new_token(value, WORD);
	add_token_back(tokens, new);
	*line += i;
	return (1);
}

int	add_quote(t_token **tokens, char **line)
{
	t_token	*new;
	char	*value;
	char	quote_char;

	quote_char = **line;
	value = ft_substr(*line, 0, 1);
	new = choose_token(value, quote_char);
	add_token_back(tokens, new);
	(*line)++;
	add_string(tokens, line, quote_char);
	if (**line == quote_char)
	{
		value = ft_substr(*line, 0, 1);
		new = choose_token(value, quote_char);
		add_token_back(tokens, new);
		(*line)++;
	}
	return (1);
}

t_token	*choose_token(char *value, char c)
{
	t_token	*new;

	new = NULL;
	if (c == '\'')
		new = new_token(value, S_QUOTE);
	else if (c == '"')
		new = new_token(value, D_QUOTE);
	else if (c == '*')
		new = new_token(value, ASTERISK);
	else if (c == '&')
		new = new_token(value, AMPERSAND);
	return (new);
}

void	clear_token(t_token **tokens)
{
	t_token	*curr_node;
	t_token	*next;

	curr_node = *tokens;
	if (!curr_node)
		return ;
	while (curr_node)
	{
		free(curr_node->value);
		next = curr_node->next;
		free(curr_node);
		curr_node = next;
	}
	*tokens = NULL;
}
