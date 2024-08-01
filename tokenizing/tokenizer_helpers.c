/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 20:01:30 by alassiqu          #+#    #+#             */
/*   Updated: 2024/08/01 18:15:12 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	*new_token(char *value, t_type type, int wd)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	gc_add(g_minishell, new_token);
	new_token->value = value;
	gc_add(g_minishell, new_token->value);
	new_token->type = type;
	new_token->hd_expand = 0;
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
		g_minishell->nb_tokens += 1;
		return ;
	}
	curr_node = *tokens;
	while (curr_node && curr_node->next)
		curr_node = curr_node->next;
	curr_node->next = new_token;
	new_token->prev = curr_node;
	g_minishell->nb_tokens += 1;
}

int	append_separator(t_token **tokens, char **line, t_type type)
{
	t_token	*token;
	char	*value;

	if (type == RR_REDIR || type == LL_REDIR || type == AND || type == OR)
		value = ft_substr(*line, 0, 2);
	else
		value = ft_substr(*line, 0, 1);
	token = new_token(value, type, 1);
	if (!token)
		return (0);
	add_token_back(tokens, token);
	(*line)++;
	if (type == RR_REDIR || type == LL_REDIR || type == AND || type == OR)
		(*line)++;
	return (1);
}

// t_token	*append_quotes(t_token **tokens, char **line, char c)
// {
// 	printf("quotes\n");
// 	t_token	*new;
// 	char	*value;
// 	char	*tmp;
// 	int		i;

// 	i = 0;
// 	tmp = (*line) + 1;
// 	while ((*line)[i] && (*line)[i] != c)
// 		i++;
// 	value = ft_substr(tmp, 0, i);
// 	(*line) += i + 1;
// 	printf("line >> %s\n", (*line));
// 	if (c == '\'')
// 		new = new_token(value, WORD, 0);
// 	else
// 		new = new_token(value, WORD, 1);
// 	return (add_token_back(tokens, new), NULL);
// }

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
	new = new_token(value, WORD, 1);
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
	token = new_token(value, WHITESPACE, 1);
	if (!token)
		return (0);
	add_token_back(tokens, token);
	(*line) += i;
	return (1);
}
