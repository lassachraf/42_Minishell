/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 20:01:30 by alassiqu          #+#    #+#             */
/*   Updated: 2024/09/06 18:42:35 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	*new_token(char *value, t_type type, int wd, int quoted)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	gc_add(g_minishell, new_token);
	new_token->value = value;
	gc_add(g_minishell, new_token->value);
	new_token->type = type;
	new_token->avoid = 0;
	new_token->hd_expand = 0;
	new_token->quoted = quoted;
	new_token->wd_expand = wd;
	new_token->next_space = 0;
	new_token->prev = NULL;
	new_token->next = NULL;
	return (new_token);
}

void	add_token_back(t_token **tokens, t_token *new_token)
{
	t_token	*curr_node;

	if (!*tokens)
	{
		*tokens = new_token;
		return ;
	}
	curr_node = *tokens;
	while (curr_node && curr_node->next)
		curr_node = curr_node->next;
	curr_node->next = new_token;
	new_token->prev = curr_node;
}

int	append_separator(t_token **tokens, char **line, t_type type)
{
	t_token	*token;
	char	*value;

	if (type == RR_REDIR || type == LL_REDIR || type == AND || type == OR)
		value = ft_substr(*line, 0, 2);
	else
		value = ft_substr(*line, 0, 1);
	token = new_token(value, type, 1, 0);
	if (!token)
		return (0);
	add_token_back(tokens, token);
	(*line)++;
	if (type == RR_REDIR || type == LL_REDIR || type == AND || type == OR)
		(*line)++;
	return (1);
}

int	append_identifier(t_token **tokens, char **line)
{
	t_token	*new;
	char	*value;
	char	*tmp;
	size_t	i;

	tmp = *line;
	i = 0;
	while (tmp[i] && !is_separator(tmp + i) && !is_quote(*(tmp + i)))
		i++;
	value = ft_substr(tmp, 0, i);
	if (!value)
		return (0);
	new = new_token(value, WORD, 1, 0);
	if (!new)
		return (0);
	*line += i;
	return (add_token_back(tokens, new), 1);
}

int	append_space(t_token **tokens, char **line)
{
	t_token	*token;
	char	*value;
	char	*tmp;
	int		i;

	i = 0;
	tmp = *line;
	while (tmp[i] && ft_isspace(tmp[i]))
		i++;
	value = ft_substr(*line, 0, i);
	if (!value)
		return (0);
	token = new_token(value, WHITESPACE, 0, 0);
	if (!token)
		return (0);
	add_token_back(tokens, token);
	(*line) += i;
	return (1);
}
