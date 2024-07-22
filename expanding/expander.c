/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 11:11:46 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/22 15:48:31 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	fill_dollar(char *s, int *i, char *new, int *j)
{
	char	*expand;
	char	*expanded;
	char	*var;
	int		k;
	int		a;

	expand = s + *i;
	k = 0;
	a = 0;
	if (!ft_strncmp(&expand[1], "\0", 1))
	{
		new[(*j)++] = s[(*i)++];
		return ;
	}
	else if (!ft_strncmp(&expand[1], "?", 1) || !ft_strncmp(&expand[1], "_", 1))
	{
		k = 1;
		expand++;
	}
	else if (!ft_strncmp(expand + 1, "$", 1) || ft_isspace(expand[1])
		|| (!ft_isalnum(expand[1]) && ft_strncmp(&expand[1], "_", 1)
		&& ft_strncmp(&expand[1], "?", 1)) || !ft_isalnum(expand[1]) || ft_isnum(expand[1]))
	{
		new[(*j)++] = s[(*i)++];
		new[(*j)++] = s[(*i)++];
		return ;
	}
	else
	{
		expand++;
		while (expand[k])
		{
			if (ft_isalnum(expand[k]) || !ft_strncmp(&expand[k], "_", 1))
				k++;
			else
				break ;
		}
	}
	var = ft_substr(expand, 0, k);
	expanded = get_env_var(g_minishell->our_env, var);
	free(var);
	(*i) += k + 1;
	if (!expanded)
		return ;
	else
	{
		while (expanded[a])
		{
			new[*j] = expanded[a++];
			(*j)++;
		}
	}
}

char	*new_value(char *s, int size)
{
	char	*new;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new = ft_malloc(g_minishell, size);
	if (!new)
		return (NULL);
	while (s[i])
	{
		if (s[i] == '$')
			fill_dollar(s, &i, new, &j);
		else
			new[j++] = s[i++];
	}
	new[j] = '\0';
	if (!new[0])
		return (NULL);
	return (new);
}

char	*helper_expander(char *s)
{
	char	*new;
	int		len;
	int		i;

	i = 0;
	len = 0;
	while (s[i])
	{
		if (s[i] == '$')
			handle_dollar(s, &i, &len);
		else
		{
			len++;
			i++;
		}
	}
	new = new_value(s, len + 1);
	return (new);
}

int	contains_space(char *s)
{
	int	i;

	i = 0;
	while (s[i] && !ft_isspace(s[i]))
		i++;
	return ((s[i] != '\0'));
}

int	ft_count_words(char const *s, char c)
{
	int	words;

	words = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			words++;
			while (*s && *s != c)
				s++;
		}
	}
	return (words);
}

void	add_token_middle(t_token **tokens, t_token *new_token,
		t_token *prev_token)
{
	if (!tokens || !new_token)
	{
		return ;
	}
	if (prev_token == NULL)
	{
		new_token->next = *tokens;
		if (*tokens)
			(*tokens)->prev = new_token;
		*tokens = new_token;
		return ;
	}
	new_token->prev = prev_token;
	new_token->next = prev_token->next;
	if (prev_token->next)
		prev_token->next->prev = new_token;
	prev_token->next = new_token;
}

void	handle_space(t_token *tokens, char *new_value)
{
	t_token	*current;
	int		i;
	int		j;
	char	*chunk;
	t_token	*new_tok;

	current = tokens;
	if (ft_count_words(new_value, ' ') < 2)
	{
		tokens->value = new_value;
		return ;
	}
	i = 0;
	remove_token(&g_minishell->tokens, current);
	while (new_value[i])
	{
		if (!ft_isspace(new_value[i]))
		{
			j = i;
			while (new_value[i] && !ft_isspace(new_value[i]))
				i++;
			chunk = ft_substr(new_value, j, (i - j));
			new_tok = new_token(chunk, WORD);
			add_token_middle(&g_minishell->tokens, new_tok, current->prev);
			current = new_tok->next;
		}
		else
			i++;
	}
}

void	check_for_value(t_token **tokens, char *new)
{
	t_token	*tmp;

	if (!new)
	{
		tmp = (*tokens)->next;
		remove_token(&g_minishell->tokens, (*tokens));
		(*tokens) = tmp;
	}
	else if (contains_space(new))
		handle_space(*tokens, new);
	else
		(*tokens)->value = new;
}

t_token	*word_helper(t_token *tokens)
{
	char	*new_value;

	new_value = NULL;
	if (tokens->prev && tokens->prev->type == LL_REDIR)
		return (tokens->next);
	if (tokens->prev && tokens->prev->type == D_QUOTE)
	{
		g_minishell->dq_flag = 1;
		new_value = helper_expander(tokens->value);
		check_for_value(&tokens, new_value);
	}
	else
	{
		new_value = helper_expander(tokens->value);
		check_for_value(&tokens, new_value);
	}
	return (tokens);
}

void	expanding(void)
{
	t_token	*tokens;

	tokens = g_minishell->tokens;
	while (tokens)
	{
		g_minishell->dq_flag = 0;
		if (tokens->type == S_QUOTE)
		{
			tokens = tokens->next;
			while (tokens && tokens->type != S_QUOTE)
				tokens = tokens->next;
			tokens = tokens->next;
		}
		else if (tokens->type == WORD && ft_strchr(tokens->value, '~'))
		{
			tokens->value = custome_path(tokens->value);
			tokens = tokens->next;
		}
		// else if (tokens->type == WORD && ft_strchr(tokens->value, '*'))
		// {
		// 	if (!(tokens->prev && (tokens->prev->type == S_QUOTE
		// 				|| tokens->prev->type == D_QUOTE)))
		// 		asterisk_expand(&g_minishell->tokens, tokens);
		// 	tokens = tokens->next;
		// }
		// else if (tokens->type == WORD && ft_strchr(tokens->value, '$'))
		// 	tokens = word_helper(tokens);
		else
			tokens = tokens->next;
	}
}

void	expander(void)
{
	expanding();
	remove_quotes(&g_minishell->tokens);
}
