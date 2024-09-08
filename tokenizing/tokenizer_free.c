/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_free.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 19:58:45 by alassiqu          #+#    #+#             */
/*   Updated: 2024/09/07 17:53:06 by baouragh         ###   ########.fr       */
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
	{
		new = new_token(ft_strdup(""), WORD, 0, 0);
		add_token_back(tokens, new);
		return (1);
	}
	value = ft_substr(*line, 0, i);
	if (quote_char == '\'')
		new = new_token(value, WORD, 0, 2);
	else
		new = new_token(value, WORD, 1, 1);
	if (ft_strchr(new->value, '*'))
		new->wd_expand = 0;
	add_token_back(tokens, new);
	*line += i;
	return (1);
}

t_token	*choose_token(int c)
{
	t_token	*token;

	token = NULL;
	if (c == '\'')
		token = new_token(ft_strdup("'"), S_QUOTE, 0, 2);
	else
		token = new_token(ft_strdup("\""), D_QUOTE, 0, 1);
	return (token);
}

int	add_quote(t_token **tokens, char **line)
{
	char	quote_char;

	quote_char = **line;
	(*line)++;
	if (!(**line) || !check_for_quote_close(line, quote_char))
		return (add_token_back(tokens, choose_token(quote_char)), 1);
	add_string(tokens, line, quote_char);
	if (**line == quote_char)
		(*line)++;
	return (1);
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
		next = curr_node->next;
		free(curr_node->value);
		free(curr_node);
		curr_node = next;
	}
	*tokens = NULL;
}
