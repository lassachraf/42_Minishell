/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 22:12:44 by alassiqu          #+#    #+#             */
/*   Updated: 2024/08/01 17:52:27 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_var(char *s, int *i)
{
	char	*expand;
	char	*value;
	char	*var;
	int		j;

	j = 0;
	expand = s + *i + 1;
	if (!ft_strncmp(expand, "\0", 1) || ft_isspace(*expand))
		return (NULL);
	if (!ft_strncmp(expand, "$", 1))
		return (*i += 2, ft_strdup("$$"));
	if (!ft_strncmp(&expand[j], "?", 1) || !ft_strncmp(&expand[j], "_", 1))
		j = 1;
	else
		while (expand[j] && (!is_quote(expand[j]) || ft_isalnum(expand[j])
				|| !ft_strncmp(&expand[j], "_", 1)))
			j++;
	var = ft_substr(expand, 0, j);
	*i += j + 1;
	value = get_env_var(g_minishell->our_env, var);
	if (value)
		return (free(var), value);
	else
		return (free(var), NULL);
}

int	check_env(char *var)
{
	if (!var)
		return (-1);
	if (!get_env_var(g_minishell->our_env, var))
	{
		return (-1);
	}
	else
		return (ft_strlen(get_env_var(g_minishell->our_env, var)));
}

void	fill_tokens(t_token *current, char *new_value)
{
	t_token	*new_tok;
	char	*chunk;
	int		i;
	int		j;

	i = 0;
	while (new_value[i])
	{
		if (!ft_isspace(new_value[i]))
		{
			j = i;
			while (new_value[i] && !ft_isspace(new_value[i]))
				i++;
			chunk = ft_substr(new_value, j, (i - j));
			new_tok = new_token(chunk, WORD, 1);
			add_token_middle(&g_minishell->tokens, new_tok, current->prev);
			current = new_tok->next;
		}
		else
			i++;
	}
}

void	handle_space(t_token *tokens, char *new_value)
{
	t_token	*current;

	current = tokens;
	if (count_words(new_value) < 2)
	{
		tokens->value = new_value;
		return ;
	}
	remove_token(&g_minishell->tokens, current);
	fill_tokens(current, new_value);
}
